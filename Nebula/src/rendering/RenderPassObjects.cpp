//
// Created by michal-swiatek on 07.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderpass/RenderPassObjects.h"

#include "core/Assert.h"

namespace nebula::rendering {

    RenderPassObjects::RenderPassObjects(const uint32_t stages)
    {
        setStages(stages);
    }

    void RenderPassObjects::setStages(const uint32_t stages)
    {
        m_stage_objects = std::vector<StageObjects>(stages);
    }

    void RenderPassObjects::addObject(const uint32_t stage, View<RenderObject> object)
    {
        NB_CORE_ASSERT(stage < m_stage_objects.size(), "Stage out of range!");
        m_stage_objects[stage].push_back(object);
    }

    const RenderPassObjects::StageObjects& RenderPassObjects::viewStageObjects(const uint32_t stage) const
    {
        NB_CORE_ASSERT(stage < m_stage_objects.size(), "Stage out of range!");
        return m_stage_objects[stage];
    }

}
