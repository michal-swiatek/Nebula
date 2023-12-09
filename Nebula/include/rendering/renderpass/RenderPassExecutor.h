//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASSEXECUTOR_H
#define RENDERPASSEXECUTOR_H

#include <optional>

#include "core/Core.h"

#include "RenderPass.h"
#include "rendering/renderer/Renderer.h"
#include "rendering/renderpass/RenderPassObjects.h"

namespace nebula::rendering {

    class NEBULA_API RenderPassExecutor
    {
    public:
        virtual ~RenderPassExecutor() = default;

        virtual void resetResources(uint32_t frame_in_flight);
        [[nodiscard]] virtual Scope<RecordedCommandBuffer> execute(const RenderPassObjects& renderpass_objects, std::optional<uint32_t> frame_in_flight = {}) const; // NOLINT(*-default-arguments)

        void setRenderer(Scope<Renderer>&& renderer);
        void setFramebuffer(const Reference<Framebuffer>& framebuffer) const;

        //  Defined in DetectPlatform.cpp
        static Scope<RenderPassExecutor> create(Scope<Renderer>&& renderer);
        static Scope<RenderPassExecutor> create(Scope<RenderPass>&& renderpass);

    protected:
        explicit RenderPassExecutor(Scope<Renderer>&& renderer);
        explicit RenderPassExecutor(Scope<RenderPass>&& renderpass);

        [[nodiscard]] virtual Scope<RecordedCommandBuffer> recordCommands(Scope<RenderCommandBuffer>&& commands, std::optional<uint32_t> frame_in_flight) const;

    private:
        Scope<Renderer> m_renderer;
        Scope<RenderPass> m_renderpass;
        Scope<RecordedCommandBuffer> m_recorded_command_buffer;
    };

}

#endif //RENDERPASSEXECUTOR_H
