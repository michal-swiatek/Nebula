//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>

#include "RenderCommand.h"
#include "memory/Allocators.h"

namespace nebula {

    namespace threads {

        class RenderThread;

    }

    namespace rendering {

        class NEBULA_API RenderPass
        {
        public:
            using PassID = uint32_t;

            RenderPass();
            virtual ~RenderPass();

            RenderPass(RenderPass&) = delete;
            RenderPass& operator=(RenderPass&) = delete;

            template <typename RenderCommand, typename... Args>
            void submit(Args&&... args)
            {
                if (RenderCommand::getStaticCategory() == cDraw)
                    m_render_queue.emplace_back(m_allocator.create<RenderCommand>(std::forward<Args>(args)...));
                else
                    m_configuration_queue.emplace_back(m_allocator.create<RenderCommand>(std::forward<Args>(args)...));
            }

            void dispatch();

        protected:
            using RenderCommandQueue = std::vector<View<RenderCommand>>;

            RenderCommandQueue m_configuration_queue;
            RenderCommandQueue m_render_queue;

            virtual void prepareQueues() {}

        private:
            memory::LinearAllocator m_allocator;
        };

        class NEBULA_API RenderPassTemplate
        {
        public:
            virtual ~RenderPassTemplate() = 0;

            template <typename PassType, typename... Args>
            void addPass(Args&&... args)
            {
                m_render_passes.emplace_back(createScope<PassType>(std::forward<Args>(args)...));
            }

            [[nodiscard]] bool isValid() const { return !m_render_passes.empty(); }

        private:
            std::vector<Scope<RenderPass>> m_render_passes{};
            uint32_t m_current_pass = 0;

            View<RenderPass> getNextPass();     //  Called by Renderer
            void dispatchRenderPasses() const;  //  Called by RenderThread

            friend class nebula::rendering::Renderer;
            friend class nebula::threads::RenderThread;
        };

    }

}

#endif //RENDERPASS_H
