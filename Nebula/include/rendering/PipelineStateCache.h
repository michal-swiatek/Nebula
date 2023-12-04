//
// Created by michal-swiatek on 04.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef CACHEDPIPELINESTATE_H
#define CACHEDPIPELINESTATE_H

#include "core/Types.h"
#include "utility/ObjectCacheManager.h"
#include "rendering/PipelineState.h"

namespace nebula::rendering {

    //  Hash functors
    struct InputAssemblyHash    { std::size_t operator() (const InputAssemblyState&) const; };
    struct RasterizationHash    { std::size_t operator() (const RasterizationState&) const; };
    struct DepthStencilHash     { std::size_t operator() (const DepthStencilState&) const; };
    struct MultisamplingHash    { std::size_t operator() (const MultisamplingState&) const; };
    struct ColorBlendingHash    { std::size_t operator() (const ColorBlendingState&) const; };
    struct GraphicsPipelineHash { std::size_t operator() (const GraphicsPipelineState&) const; };

    //  Typedefs
    using GraphicsPipelineMapper = ObjectCacheManager<GraphicsPipelineState, GraphicsPipelineHash>;
    using GraphicsPipelineHandle = GraphicsPipelineMapper::HandleType;
    static constexpr GraphicsPipelineHandle NULL_GRAPHICS_PIPELINE = GraphicsPipelineMapper::NULL_HANDLE;

    class PipelineStateCache
    {
    public:
        static GraphicsPipelineHandle getPipelineHandle(const GraphicsPipelineState& pipeline_state);
        static const GraphicsPipelineState& getPipelineState(GraphicsPipelineHandle pipeline_handle);

        static void init();
        static void shutdown();

    private:
        static Scope<GraphicsPipelineMapper> s_graphics_pipeline_handle_mapper;
    };

}

#endif //CACHEDPIPELINESTATE_H
