//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATION_H
#define NEBULAENGINE_APPLICATION_H

#include <string>

#include "Core.h"

#include "events/Event.h"
#include "events/EventManager.h"

namespace nebula {

    class NEBULA_API Application
    {
    public:
        explicit Application(std::string ph_1, const std::string& logger_name = "APP");
        virtual ~Application() = default;

        void onEvent(Event& event);

        void run();

    private:
        std::string m_name;

        EventManager m_event_manager;
    };

    Application* createApplication(int argc, char** argv);

}

#endif //NEBULAENGINE_APPLICATION_H
