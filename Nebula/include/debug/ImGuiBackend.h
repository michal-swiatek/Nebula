//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef IMGUIBACKEND_H
#define IMGUIBACKEND_H

#include "rendering/renderpass/RenderPass.h"
#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula {

    namespace threads { class MainRenderThread; }

    class ImGuiBackend
    {
    public:
        virtual ~ImGuiBackend() = default;

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;

        virtual void begin() = 0;
        virtual void end(void* command_buffer_handle) = 0;

        static Scope<ImGuiBackend> create(rendering::RenderPass& renderpass);

    protected:
        ImGuiBackend() = default;

        //  Defined in ImGuiLayer.cpp
        static void init();
        static void shutdown();
        friend nebula::threads::MainRenderThread;
    };

}

#endif //IMGUIBACKEND_H
