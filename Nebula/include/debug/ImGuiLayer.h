//
// Created by michal-swiatek on 08.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_IMGUILAYER_H
#define NEBULAENGINE_IMGUILAYER_H

#include <mutex>

#include "core/Layer.h"
#include "core/Timer.h"

namespace nebula {

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void onAttach() override;
        void onDetach() override;
        void onEvent(Event &event) override;

        void onImGuiRender() override;

        static void begin();
        static void end();

        void setBlockEvents(bool block) { m_block_events = block; }

    private:
        bool m_block_events = true;

        static int s_counter;
        static std::mutex s_mutex;

    //  Debug ImGui windows
    private:
        void performanceOverlay();
        void fpsSection();

        Timer m_frame_timer{};
    };

}

#endif //NEBULAENGINE_IMGUILAYER_H
