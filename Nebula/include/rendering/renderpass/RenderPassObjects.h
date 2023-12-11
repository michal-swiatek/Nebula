//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef RENDERPASSOBJECTS_H
#define RENDERPASSOBJECTS_H

#include <vector>

#include "core/Types.h"
#include "rendering/RenderObject.h"

namespace nebula::rendering {

    class RenderPassObjects
    {
    public:
        using StageObjects = std::vector<View<RenderObject>>;

        RenderPassObjects() = default;
        explicit RenderPassObjects(uint32_t stages);

        void setStages(uint32_t stages);
        void addObject(uint32_t stage, View<RenderObject> object);

        [[nodiscard]] const StageObjects& viewStageObjects(uint32_t stage) const;

    private:
        std::vector<StageObjects> m_stage_objects;
    };

}

#endif //RENDERPASSOBJECTS_H
