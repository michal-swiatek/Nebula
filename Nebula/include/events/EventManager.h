//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_EVENTMANAGER_H
#define NEBULAENGINE_EVENTMANAGER_H

#include <mutex>
#include <vector>
#include <functional>

#include "Event.h"
#include "core/Types.h"
#include "core/LayerStack.h"
#include "memory/Allocators.h"

namespace nebula {

    class NEBULA_API EventManager
    {
    public:
        using EventCallback = std::function<void(Event&)>;

        EventManager(LayerStack& layer_stack, EventCallback application_callback, std::size_t event_memory_size);
        ~EventManager();

        void dispatchEvents();
        void broadcastEvent(Event& event) const;

        template <typename T, typename... Args>
        void queueEvent(Args&&... args)
        {
            auto event = new (m_allocator.allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
            std::lock_guard<std::mutex> lock{m_mutex};
            m_events.emplace_back(std::move(event));
        }

    private:
        std::mutex m_mutex;

        LayerStack& m_layer_stack;
        EventCallback m_application_callback;
        std::vector<Event*> m_events{};
        memory::LinearAllocator m_allocator;
    };

}

#endif //NEBULAENGINE_EVENTMANAGER_H
