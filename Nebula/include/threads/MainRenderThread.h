//
// Created by michal-swiatek on 05.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef MAINRENDERTHREAD_H
#define MAINRENDERTHREAD_H

#include "threads/SecondaryThread.h"

#include "core/Types.h"
#include "core/Application.h"

#include "rendering/RenderContext.h"
#include "rendering/renderer/Renderer.h"

namespace nebula::threads {

    class MainRenderThread final : public SecondaryThread
    {
    public:
        MainRenderThread() : m_application(Application::get()) {}

    private:
        Application& m_application;

        Scope<rendering::Renderer> m_renderer;
        Scope<rendering::RenderContext> m_render_context;

        void init() override;
        void shutdown() override;

        void mainLoopBody() override;
    };

}

#endif //MAINRENDERTHREAD_H
