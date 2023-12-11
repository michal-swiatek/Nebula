//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDEROBJECTVISITOR_H
#define RENDEROBJECTVISITOR_H

#include "core/Core.h"

namespace nebula::rendering {

    class RenderObject;
    class ImGuiRenderObject;

    class NEBULA_API RenderObjectVisitor
    {
    public:
        virtual ~RenderObjectVisitor() = default;

        virtual void draw(const ImGuiRenderObject& imgui_layer) {}
    };

}

#endif //RENDEROBJECTVISITOR_H
