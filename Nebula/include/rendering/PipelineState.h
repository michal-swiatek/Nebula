//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef GRAPHICSPIPELINESTATE_H
#define GRAPHICSPIPELINESTATE_H

#include "core/Core.h"
#include "core/Assert.h"

#include "Shader.h"
#include "rendering/TextureFormats.h"

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
        cTriangleStrip,
        cTriangleFan
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

        friend bool operator == (const InputAssemblyState&, const InputAssemblyState&) = default;
    };

    struct NEBULA_API RasterizationState
    {
        bool enabled = true;
        bool face_clockwise = true;

        CullMode cull_mode = CullMode::cBack;
        PolygonMode polygon_mode = PolygonMode::cFill;

        friend bool operator == (const RasterizationState&, const RasterizationState&) = default;
    };

    struct NEBULA_API DepthStencilState
    {
        bool enabled = true;
        bool depth_clamp = false;

        bool depth_bias_enabled = false;
        float depth_bias_constant = 0.0f;
        float depth_bias_clamp = 0.0f;
        float depth_bias_slope = 0.0f;

        friend bool operator == (const DepthStencilState&, const DepthStencilState&);
    };

    struct NEBULA_API MultisamplingState
    {
        bool enabled = false;
        bool alpha_to_one_enable = false;
        bool alpha_to_coverage_enable = false;

        uint32_t sample_mask = 0;
        float min_sample_shading = 1.0f;
        TextureSampling samples = TextureSampling::cTexture1Sample;

        friend bool operator == (const MultisamplingState&, const MultisamplingState&);
    };

    struct NEBULA_API ColorBlendingState
    {
        bool enabled = false;
        //  TODO: Implement

        friend bool operator == (const ColorBlendingState&, const ColorBlendingState&) = default;
    };

    struct NEBULA_API GraphicsPipelineState
    {
        View<Shader> shader = nullptr;
        VertexLayout vertex_layout{};
        PipelineLayout pipeline_layout{};

        InputAssemblyState input_assembly{};
        RasterizationState rasterization{};
        MultisamplingState multisampling{};

        DepthStencilState depth_stencil{};
        ColorBlendingState color_blending{};

        DynamicState dynamic_state_flags = static_cast<DynamicState>(cViewport | cScissor | cLineWidth);

        explicit GraphicsPipelineState(const View<Shader> shader) : shader(shader)
        {
            NB_CORE_ASSERT(this->shader, "GraphicsPipelineState requires shader!");
        }

        friend bool operator == (const GraphicsPipelineState&, const GraphicsPipelineState&);
    };

    //  Hash functors
    struct InputAssemblyHash    { std::size_t operator() (const InputAssemblyState&) const; };
    struct RasterizationHash    { std::size_t operator() (const RasterizationState&) const; };
    struct DepthStencilHash     { std::size_t operator() (const DepthStencilState&) const; };
    struct MultisamplingHash    { std::size_t operator() (const MultisamplingState&) const; };
    struct ColorBlendingHash    { std::size_t operator() (const ColorBlendingState&) const; };
    struct GraphicsPipelineHash { std::size_t operator() (const GraphicsPipelineState&) const; };

}

#endif //GRAPHICSPIPELINESTATE_H
