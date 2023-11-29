//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLRECORDEDBUFFER_H
#define OPENGLRECORDEDBUFFER_H

#include "rendering/commands/RenderCommandBuffer.h"

namespace nebula::rendering {

    class OpenGLRecordedBuffer final : public RecordedCommandBuffer
    {
    public:
        explicit OpenGLRecordedBuffer(Scope<RenderCommandBuffer>&& command_buffer) : m_commands(std::move(command_buffer)) {}

        void* getBufferHandle() override
        {
            return m_commands.get();
        }

    private:
        Scope<RenderCommandBuffer> m_commands = nullptr;
    };

}

#endif //OPENGLRECORDEDBUFFER_H
