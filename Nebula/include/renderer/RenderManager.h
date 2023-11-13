//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <vector>

#include "core/Types.h"
#include "RenderPass.h"
#include "RenderContext.h"

namespace nebula::rendering {

    class Renderer;

    namespace impl {

        class NEBULA_API RenderManager
        {
        public:
            RenderManager();

            View<RenderPass> addPass()
            {
                m_render_passes.emplace_back(createScope<RenderPass>());
                return m_render_passes.back().get();
            }

            template <typename PassType, typename... Args>
            View<RenderPass> addPass(Args&&... args)
            {
                m_render_passes.emplace_back(createScope<PassType>(std::forward<Args>(args)...));
                return m_render_passes.back().get();
            }

            [[nodiscard]] View<RenderPass> getPassByID(RenderPass::PassID id) const;
            [[nodiscard]] const std::vector<Scope<RenderPass>>& getPasses() const { return m_render_passes; }

        private:
            std::vector<Scope<RenderPass>> m_render_passes{};

            void dispatchPasses();

            friend class nebula::rendering::Renderer;
        };

    }

}

#endif //RENDERMANAGER_H
