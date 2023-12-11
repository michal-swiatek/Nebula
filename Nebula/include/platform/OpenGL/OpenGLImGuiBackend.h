//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLIMGUIBACKEND_H
#define OPENGLIMGUIBACKEND_H

#include "debug/ImGuiBackend.h"

namespace nebula {

    class OpenGlImGuiBackend final : public ImGuiBackend
    {
    public:
        explicit OpenGlImGuiBackend(rendering::RenderPass& renderpass);

        void onAttach() override;
        void onDetach() override;

        void begin() override;
        void end(void* command_buffer_handle) override;

    private:
        rendering::RenderPass& m_renderpass;

        static void init();
        static void shutdown();
        friend nebula::ImGuiBackend;
    };

}

#endif //OPENGLIMGUIBACKEND_H
