//
// Created by michal-swiatek on 05.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef MAINRENDERTHREAD_H
#define MAINRENDERTHREAD_H

#include <debug/ImGuiLayer.h>

#include "threads/SecondaryThread.h"

#include "core/Types.h"
#include "core/Application.h"

#include "rendering/RenderObject.h"
#include "rendering/RenderContext.h"
#include "rendering/renderpass/RenderPassExecutor.h"

namespace nebula::threads {

    class MainRenderThread final : public SecondaryThread
    {
    public:
        MainRenderThread();

    private:
        Application& m_application;
        Scope<rendering::RenderContext> m_render_context;

        bool m_vsync = true;
        ImGuiLayer* m_im_gui_layer = nullptr;

        Scope<rendering::ImGuiRenderObject> m_imgui_object;
        rendering::RenderPassObjects m_renderpass_objects;
        Scope<rendering::RenderPassExecutor> m_renderpass_executor;

        void mainLoopBody() override;

        void init() override;
        void shutdown() override;

        void reloadSwapchain();
        void initFinalRenderpass(bool setup_imgui_layer = false);
    };

}

#endif //MAINRENDERTHREAD_H
