//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include <ranges>

#include "events/EventManager.h"

namespace nebula {

    EventManager::EventManager(LayerStack& layer_stack, EventManager::EventCallback application_callback)
            : m_layer_stack(layer_stack), m_application_callback(std::move(application_callback)) {}

    void EventManager::broadcastEvent(Event& event)
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
        for (const auto& event : m_events)
            broadcastEvent(*event);

        m_events.clear();
    }
}
