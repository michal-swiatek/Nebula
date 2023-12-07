//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDEROBJECTVISITOR_H
#define RENDEROBJECTVISITOR_H

#include "core/Core.h"
#include "RenderObject.h"

namespace nebula::rendering {

    class NEBULA_API RenderObjectVisitor
    {
    public:
        virtual ~RenderObjectVisitor() = default;

        void draw(const RenderObject& render_object) { render_object.accept(*this); }
    };

}

#endif //RENDEROBJECTVISITOR_H
