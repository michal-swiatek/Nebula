//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERCOMMANDBUFFER_H
#define RENDERCOMMANDBUFFER_H

#include <vector>

#include "core/Core.h"
#include "memory/Allocators.h"
#include "RenderCommand.h"

namespace nebula::rendering {


    class NEBULA_API RenderCommandBuffer
    {
    public:
        ~RenderCommandBuffer();

        void reset();

        template <typename RenderCommand, typename... Args>
        void submit(Args&&... args)
        {
            m_commands.push_back(m_allocator.create<RenderCommand>(std::forward<Args>(args)...));
        }

        template <typename RenderCommand, typename... Args>
        void replace(const int index, Args&&... args)
        {
            RenderCommand* new_command = m_allocator.create<RenderCommand>(std::forward<Args>(args)...);
            m_commands[index] = new_command;
        }

        static Scope<RenderCommandBuffer> create();

    private:
        explicit RenderCommandBuffer(size_t buffer_size);

        memory::LinearAllocator m_allocator{};
        std::vector<RenderCommand*> m_commands{};
    };

}

#endif //RENDERCOMMANDBUFFER_H
