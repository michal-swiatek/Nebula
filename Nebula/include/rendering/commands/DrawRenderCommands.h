//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef DRAWRENDERCOMMANDS_H
#define DRAWRENDERCOMMANDS_H

#include "RenderCommand.h"

namespace nebula::rendering {

    struct NEBULA_API DrawDummyIndicesCommand final : RenderCommand
    {
        uint32_t num_indices{};

        explicit DrawDummyIndicesCommand(uint32_t num_indices) : num_indices(num_indices) {}
        void accept(RenderCommandVisitor& command_visitor) override { command_visitor.visit(*this); }
    };

}

#endif //DRAWRENDERCOMMANDS_H
