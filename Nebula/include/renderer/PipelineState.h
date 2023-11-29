//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef GRAPHICSPIPELINESTATE_H
#define GRAPHICSPIPELINESTATE_H

#include "core/Core.h"
#include "renderer/TextureFormats.h"

namespace nebula::rendering {

    enum DynamicState : uint8_t
    {
        cViewport = 1,
        cScissor = 2,
        cLineWidth = 4,
        cDepthBias = 8,
        cStencilMask = 16,
        cBlendConstant = 32,
    };

    enum class GeometryTopology : uint8_t
    {
        cPointList,
        cLineList,
        cLineStrip,
        cTriangleList,
        cTriangleStrip
    };

    enum class PolygonMode : uint8_t
    {
        cFill,
        cLine,
        cPoint
    };

    enum class CullMode : uint8_t
    {
        cNone,
        cBack,
        cFront,
        cBackAndFront
    };

    struct NEBULA_API ShaderLayout
    {
        //  TODO: Implement!!!
    };

    struct NEBULA_API VertexLayout
    {
        //  TODO: Implement!!!
    };

    struct NEBULA_API PipelineLayout
    {
        //  TODO: Implement!!!
    };

    struct NEBULA_API InputAssemblyState
    {
        GeometryTopology topology = GeometryTopology::cTriangleList;
        bool strip_restart = false;
    };

    struct NEBULA_API RasterizationState
    {
        bool enabled = true;
        bool face_clockwise = true;

        CullMode cull_mode = CullMode::cBack;
        PolygonMode polygon_mode = PolygonMode::cFill;
    };

    struct NEBULA_API DepthStencilState
    {
        bool depth_clamp = false;

        bool depth_bias_enabled = false;
        float depth_bias_constant = 0.0f;
        float depth_bias_clamp = 0.0f;
        float depth_bias_slope = 0.0f;
    };

    struct NEBULA_API MultisamplingState
    {
        bool enabled = false;
        bool alpha_to_one = false;
        bool alpha_to_coverage_enable = false;

        uint32_t sample_mask = 0;
        float min_sample_shading = 1.0f;
        TextureSampling samples = TextureSampling::cTexture1Sample;
    };

    struct NEBULA_API ColorBlendingState
    {
        bool enabled = false;
        //  TODO: Implement
    };

    struct NEBULA_API GraphicsPipelineState
    {
        ShaderLayout shader_layout{};
        VertexLayout vertex_layout{};
        PipelineLayout pipeline_layout{};

        InputAssemblyState input_assembly{};
        RasterizationState rasterization{};
        MultisamplingState multisampling{};

        DepthStencilState depth_stencil{};
        ColorBlendingState color_blending{};

        uint8_t dynamic_state_mask = cViewport | cScissor | cLineWidth;
    };

}

#endif //GRAPHICSPIPELINESTATE_H
