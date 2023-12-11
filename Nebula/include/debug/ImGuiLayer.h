//
// Created by michal-swiatek on 08.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_IMGUILAYER_H
#define NEBULAENGINE_IMGUILAYER_H

#include "core/Layer.h"
#include "core/Timer.h"

#include "ImGuiBackend.h"
#include "rendering/renderpass/RenderPass.h"

namespace nebula {

    class ImGuiLayer : public Layer
    {
    public:
        explicit ImGuiLayer(rendering::RenderPass& renderpass);
        ~ImGuiLayer() override;

        void onAttach() override;
        void onDetach() override;
        void onEvent(Event &event) override;

        void onImGuiRender() override;
        void reloadBackend(rendering::RenderPass& renderpass);

        static void begin();
        static void end(void* command_buffer_handle);

        void setBlockEvents(const bool block) { m_block_events = block; }

    protected:
        virtual void apiSection() = 0;

    private:
        bool m_block_events = true;
        Scope<ImGuiBackend> m_backend = nullptr;

        static int s_counter;
        static ImGuiBackend* s_backend;

        //  Debug ImGui windows
        void performanceOverlay();
        void fpsSection();

        Timer m_frame_timer{};
    };

}

#endif //NEBULAENGINE_IMGUILAYER_H
