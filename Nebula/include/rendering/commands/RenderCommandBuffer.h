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

    class VulkanRecordedBuffer;
    class OpenGLRecordedBuffer;

    class NEBULA_API RecordedCommandBuffer
    {
    public:
        virtual ~RecordedCommandBuffer() = default;

        virtual void* getBufferHandle() = 0;
        [[nodiscard]] virtual const std::vector<RenderCommand*>& viewCommands() const = 0;

    protected:
        RecordedCommandBuffer() = default;

        friend class nebula::rendering::VulkanRecordedBuffer;
        friend class nebula::rendering::OpenGLRecordedBuffer;
    };

    class NEBULA_API RenderCommandBuffer final : public RecordedCommandBuffer
    {
    public:
        ~RenderCommandBuffer() override;

        void* getBufferHandle() override;

        void reset();
        [[nodiscard]] const std::vector<RenderCommand*>& viewCommands() const override;

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
