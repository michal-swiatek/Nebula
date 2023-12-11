//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "RenderObjectVisitor.h"

namespace nebula::rendering {

    class RenderObject
    {
    public:
        virtual ~RenderObject() = default;

        virtual void accept(RenderObjectVisitor& visitor) const = 0;
    };

    class ImGuiRenderObject final : public RenderObject
    {
    public:
        void accept(RenderObjectVisitor& visitor) const override { visitor.draw(*this); }
    };

}

#endif //RENDEROBJECT_H
