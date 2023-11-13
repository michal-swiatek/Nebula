//
// Created by michal-swiatek on 12.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>

#include "RenderCommand.h"
#include "memory/Allocators.h"

namespace nebula::rendering {

    class NEBULA_API RenderPass
    {
    public:
        using PassID = uint32_t;

        explicit RenderPass();
        virtual ~RenderPass() = default;

        template <typename RenderCommand, typename... Args>
        void submit(Args&&... args)
        {
            if (RenderCommand::getStaticCategory() == cDraw)
                m_render_queue.emplace_back(m_allocator.create<RenderCommand>(std::forward<Args>(args)...));
            else
                m_configuration_queue.emplace_back(m_allocator.create<RenderCommand>(std::forward<Args>(args)...));
        }

        void dispatch();

        [[nodiscard]] PassID getID() const { return m_id; }

    protected:
        using RenderCommandQueue = std::vector<View<RenderCommand>>;

        RenderCommandQueue m_configuration_queue;
        RenderCommandQueue m_render_queue;

        virtual void prepareQueues() {}

    private:
        PassID m_id{};
        memory::LinearAllocator m_allocator;

        static PassID s_id_counter;
    };

}

#endif //RENDERPASS_H
