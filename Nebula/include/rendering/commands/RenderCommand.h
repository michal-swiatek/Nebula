//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "core/Core.h"
#include "RenderCommandVisitor.h"

namespace nebula::rendering {

    class RenderCommandVisitor;

    struct NEBULA_API RenderCommand
    {
        virtual ~RenderCommand() = default;

        virtual void accept(RenderCommandVisitor& command_visitor) = 0;
    };

}

#endif //RENDERCOMMAND_H
