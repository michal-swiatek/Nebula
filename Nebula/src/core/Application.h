//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATION_H
#define NEBULAENGINE_APPLICATION_H

#include <string>

#include "Core.h"

namespace nebula {

    class NEBULA_API Application
    {
    public:
        explicit Application(std::string name);
        virtual ~Application() = default;

        void run();

    private:
        std::string m_name;
    };

    Application* createApplication();

}

#endif //NEBULAENGINE_APPLICATION_H
