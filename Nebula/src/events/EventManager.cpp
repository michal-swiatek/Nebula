//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "events/EventManager.h"

namespace nebula {

    EventManager::EventManager(EventManager::EventCallback application_callback)
            : m_application_callback(std::move(application_callback)) {}

    void EventManager::broadcastEvent(Event& event)
    {
        m_application_callback(event);
    }

    void EventManager::dispatchEvents()
    {
        for (const auto& event : m_events)
        {
            m_application_callback(*event);
        }

        m_events.clear();
    }
}
