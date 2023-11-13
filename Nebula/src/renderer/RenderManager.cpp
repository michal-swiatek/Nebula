//
// Created by michal-swiatek on 13.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/RenderManager.h"

namespace nebula::rendering::impl {

    RenderManager::RenderManager()
    {
        addPass();
    }

    void RenderManager::render(impl::RenderContext& context)
    {
        context.bind();

        for (const auto& pass : m_render_passes)
            pass->dispatch();
    }

    View<RenderPass> RenderManager::getPassByID(RenderPass::PassID id) const
    {
        auto it = std::ranges::find_if(m_render_passes, [id](const auto& pass){ return pass->getID() == id; });
        if (it != m_render_passes.end())
            return it->get();

        return nullptr;
    }

}
