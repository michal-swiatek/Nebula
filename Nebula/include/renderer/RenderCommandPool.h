//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMANDPOOL_H
#define RENDERCOMMANDPOOL_H

#include "core/Types.h"

#include "RenderCommandBuffer.h"

namespace nebula::rendering {

    class Renderer;
    class RenderCommandBuffer;

    //  TODO: Implement memory pool
    class RenderCommandPool
    {
    public:
        explicit RenderCommandPool(const size_t command_buffer_size) : m_command_buffer_size(command_buffer_size) {}
        virtual ~RenderCommandPool() = default;

        virtual Scope<RenderCommandBuffer> createBuffer()
        {
            return createScope<RenderCommandBuffer>(m_command_buffer_size);
        }

        virtual void reset() {}

    protected:
        size_t m_command_buffer_size;

    private:
        static Scope<RenderCommandPool> create();   //  Accessed by Renderer
        friend class nebula::rendering::Renderer;
    };

}

#endif //RENDERCOMMANDPOOL_H
