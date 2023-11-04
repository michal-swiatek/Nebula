//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_EVENTMANAGER_H
#define NEBULAENGINE_EVENTMANAGER_H

#include <vector>
#include <functional>

#include "Event.h"
#include "core/Types.h"
#include "core/LayerStack.h"

namespace nebula {

    class NEBULA_API EventManager
    {
    public:
        using EventCallback = std::function<void(Event&)>;

        EventManager(LayerStack& layer_stack, EventCallback application_callback);

        void dispatchEvents();
        void broadcastEvent(Event& event);

        template <typename T, typename... Args>
        void queueEvent(Args&&... args)
        {
            auto event = std::make_unique<T>(std::forward<Args>(args)...);    //  TODO: Use linear allocator
            m_events.emplace_back(std::move(event));
        }

    private:
        LayerStack& m_layer_stack;
        EventCallback m_application_callback;
        std::vector<Scope<Event>> m_events{};
    };

}

#endif //NEBULAENGINE_EVENTMANAGER_H
