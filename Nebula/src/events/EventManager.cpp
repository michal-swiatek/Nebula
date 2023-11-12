//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "events/EventManager.h"

#include <ranges>

#include "memory/MemoryManager.h"

namespace nebula {

    EventManager::EventManager(
        LayerStack& layer_stack,
        EventCallback application_callback,
        std::size_t event_memory_size
    ) :
            m_layer_stack(layer_stack),
            m_application_callback(std::move(application_callback)),
            m_allocator(memory::MemoryManager::requestMemory(event_memory_size), event_memory_size)
    {}

    EventManager::~EventManager()
    {
        m_allocator.clear();
    }

    void EventManager::broadcastEvent(Event& event) const
    {
        m_application_callback(event);

        for (const auto& it : std::ranges::reverse_view(m_layer_stack))
        {
            if (event.handled)
                break;
            it->onEvent(event);
        }
    }

    void EventManager::dispatchEvents()
    {
        //  Events don't get destroyed, so their destructors aren't called
        for (const auto& event : m_events)
            broadcastEvent(*event);

        m_events.clear();
        m_allocator.clear();
    }

}
