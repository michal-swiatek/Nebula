//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLIMGUILAYER_H
#define OPENGLIMGUILAYER_H

#include "debug/ImGuiLayer.h"

namespace nebula {

    class OpenGLImGuiLayer final : public ImGuiLayer
    {
    public:
        explicit OpenGLImGuiLayer(rendering::RenderPass& renderpass);

    private:
        void apiSection() override;
    };

}

#endif //OPENGLIMGUILAYER_H
