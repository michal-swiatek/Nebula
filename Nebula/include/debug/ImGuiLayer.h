//
// Created by michal-swiatek on 08.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_IMGUILAYER_H
#define NEBULAENGINE_IMGUILAYER_H

#include "core/Core.h"
#include "core/Layer.h"

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
    };

}

#endif //NEBULAENGINE_IMGUILAYER_H
