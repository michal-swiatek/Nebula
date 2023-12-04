//
// Created by michal-swiatek on 03.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/PipelineState.h"

#include <boost/functional/hash.hpp>

namespace nebula::rendering {

    //////////////////////////////////////////////////////////////////
    /////////  Hash functors  ////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    template <typename HashFunctor, typename T>
    void hash_combine(std::size_t& seed, const T& v)
    {
        seed = boost::hash_detail::hash_mix(seed + 0x9e3779b9 + HashFunctor()(v));
    }

    std::size_t InputAssemblyHash::operator() (const InputAssemblyState& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.topology);
        boost::hash_combine(seed, state.strip_restart);

        return seed;
    }

    std::size_t RasterizationHash::operator() (const RasterizationState& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.enabled);
        boost::hash_combine(seed, state.face_clockwise);
        boost::hash_combine(seed, state.cull_mode);
        boost::hash_combine(seed, state.polygon_mode);

        return seed;
    }

    std::size_t DepthStencilHash::operator() (const DepthStencilState& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.enabled);
        boost::hash_combine(seed, state.depth_clamp);
        boost::hash_combine(seed, state.depth_bias_enabled);
        boost::hash_combine(seed, state.depth_bias_constant);
        boost::hash_combine(seed, state.depth_bias_clamp);
        boost::hash_combine(seed, state.depth_bias_slope);

        return seed;
    }

    std::size_t MultisamplingHash::operator() (const MultisamplingState& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.enabled);
        boost::hash_combine(seed, state.alpha_to_one_enable);
        boost::hash_combine(seed, state.alpha_to_coverage_enable);
        boost::hash_combine(seed, state.sample_mask);
        boost::hash_combine(seed, state.min_sample_shading);
        boost::hash_combine(seed, state.samples);

        return seed;
    }

    std::size_t ColorBlendingHash::operator() (const ColorBlendingState& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.enabled);

        return seed;
    }

    std::size_t GraphicsPipelineHash::operator() (const GraphicsPipelineState& state) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, state.shader->getName());
        hash_combine<InputAssemblyHash>(seed, state.input_assembly);
        hash_combine<RasterizationHash>(seed, state.rasterization);
        hash_combine<MultisamplingHash>(seed, state.multisampling);
        hash_combine<DepthStencilHash>(seed, state.depth_stencil);
        hash_combine<ColorBlendingHash>(seed, state.color_blending);
        boost::hash_combine(seed, state.dynamic_state_flags);

        return seed;
    }

    //////////////////////////////////////////////////////////////////
    /////////  Comparison operators  /////////////////////////////////
    //////////////////////////////////////////////////////////////////

    bool compareFloats(const float x, const float y)
    {
        static constexpr float epsilon = 1e-6f;
        return std::abs(x - y) <= epsilon;
    }

    bool operator == (const DepthStencilState& lhs, const DepthStencilState& rhs)
    {
        return  lhs.enabled == rhs.enabled &&
                lhs.depth_clamp == rhs.depth_clamp &&
                lhs.depth_bias_enabled == rhs.depth_bias_enabled &&
                compareFloats(lhs.depth_bias_constant, rhs.depth_bias_constant) &&
                compareFloats(lhs.depth_bias_clamp, rhs.depth_bias_clamp) &&
                compareFloats(lhs.depth_bias_slope, rhs.depth_bias_slope);
    }

    bool operator == (const MultisamplingState& lhs, const MultisamplingState& rhs)
    {
        return  lhs.enabled == rhs.enabled &&
                lhs.samples == rhs.samples &&
                lhs.sample_mask == rhs.sample_mask &&
                lhs.alpha_to_one_enable == rhs.alpha_to_one_enable &&
                lhs.alpha_to_coverage_enable == rhs.alpha_to_coverage_enable &&
                compareFloats(lhs.min_sample_shading, rhs.min_sample_shading);
    }

    bool operator == (const GraphicsPipelineState& lhs, const GraphicsPipelineState& rhs)
    {
        return  lhs.shader->getName() == rhs.shader->getName() &&
                lhs.depth_stencil == rhs.depth_stencil &&
                lhs.color_blending == rhs.color_blending &&
                lhs.rasterization == rhs.rasterization &&
                lhs.input_assembly == rhs.input_assembly &&
                lhs.multisampling == rhs.multisampling &&
                lhs.dynamic_state_flags == rhs.dynamic_state_flags;
    }

}
