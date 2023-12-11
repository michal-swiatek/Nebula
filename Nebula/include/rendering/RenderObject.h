//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "core/Core.h"
#include "RenderObjectVisitor.h"

namespace nebula::rendering {

    class NEBULA_API RenderObject
    {
    public:
        virtual ~RenderObject() = default;

        virtual void accept(RenderObjectVisitor& visitor) const = 0;
    };

    class NEBULA_API ImGuiRenderObject final : public RenderObject
    {
    public:
        void accept(RenderObjectVisitor& visitor) const override { visitor.draw(*this); }
    };

    class NEBULA_API DummyVerticesRenderObject final : public RenderObject
    {
    public:
        explicit DummyVerticesRenderObject(const uint32_t num_indices) : m_num_indices(num_indices) {}
        void accept(RenderObjectVisitor& visitor) const override { visitor.draw(*this); }

        [[nodiscard]] uint32_t getNumIndices() const { return m_num_indices; }

    private:
        uint32_t m_num_indices{};
    };

}

#endif //RENDEROBJECT_H
