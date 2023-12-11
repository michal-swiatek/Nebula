//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDEROBJECTVISITOR_H
#define RENDEROBJECTVISITOR_H

#include "core/Core.h"

namespace nebula::rendering {

    class ImGuiRenderObject;
    class DummyVerticesRenderObject;

    class NEBULA_API RenderObjectVisitor
    {
    public:
        virtual ~RenderObjectVisitor() = default;

        virtual void draw(const ImGuiRenderObject& imgui_layer) {}
        virtual void draw(const DummyVerticesRenderObject& imgui_layer) {}
    };

}

#endif //RENDEROBJECTVISITOR_H
