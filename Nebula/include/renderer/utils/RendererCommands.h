//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERERCOMMANDS_H
#define RENDERERCOMMANDS_H

#include <glm/glm.hpp>

#include "renderer/RendererAPI.h"
#include "renderer/RenderCommand.h"

namespace nebula::rendering {

    class NEBULA_API SetClearColorCommand : public RenderCommand
    {
    public:
        explicit SetClearColorCommand(glm::vec4 color) : m_color(color) {}

        void execute() override { s_renderer_api->setClearColor(m_color); }
        RENDER_COMMAND_CLASS_CATEGORY(cConfigure);

    private:
        glm::vec4 m_color;
    };

    class NEBULA_API ClearCommand : public RenderCommand
    {
    public:
        explicit ClearCommand(ClearBufferType flags = static_cast<ClearBufferType>(cColorBuffer | cDepthBuffer)) : m_flags(flags) {}

        void execute() override { s_renderer_api->clear(m_flags); }
        RENDER_COMMAND_CLASS_CATEGORY(cClear);

    protected:
        ClearBufferType m_flags;
    };

    class NEBULA_API ClearColorCommand final : public ClearCommand
    {
    public:
        explicit ClearColorCommand(
            glm::vec4 color,
            bool clear_depth = true
        ) :
                ClearCommand(static_cast<ClearBufferType>(cColorBuffer | (clear_depth ? cDepthBuffer : 0))),
                m_color(color)
        {}

        ClearColorCommand(
            float red,
            float green,
            float blue,
            float alpha,
            bool clear_depth = true
        ) :
                ClearCommand(static_cast<ClearBufferType>(cColorBuffer | (clear_depth ? cDepthBuffer : 0))),
                m_color({red, green, blue, alpha})
        {}

        void execute() override
        {
            s_renderer_api->setClearColor(m_color);
            ClearCommand::execute();
        }

        RENDER_COMMAND_CLASS_CATEGORY(static_cast<RenderCommandCategory>(cClear | cConfigure));

    private:
        glm::vec4 m_color;
    };

}

#endif //RENDERERCOMMANDS_H
