//
// Created by michal-swiatek on 10.06.2022.
// Github: https://github.com/michal-swiatek/
//

#ifndef NEBULA_APPLICATION_H
#define NEBULA_APPLICATION_H

#include "Engine/NebulaAPI.h"

namespace nebula {

    class NEBULA_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    //  Define application instance - implemented by user
    Application* CreateApplication();

}

#endif //NEBULA_APPLICATION_H
