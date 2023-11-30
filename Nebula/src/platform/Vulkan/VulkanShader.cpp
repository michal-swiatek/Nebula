//
// Created by michal-swiatek on 30.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanShader.h"

#include "core/Assert.h"

namespace nebula::rendering {

    VulkanShader::VulkanShader(
        const std::string& name,
        const ShaderTemplate& shader_template
    ) :
            Shader(name, shader_template)
    {
        if (std::holds_alternative<VertexShader>(shader_template))
            buildVertexShader(std::get<VertexShader>(shader_template));
    }

    VulkanShader::~VulkanShader()
    {
        for (const auto& [stage, module] : m_shader_stages)
            vkDestroyShaderModule(VulkanAPI::getDevice(), module, nullptr);
    }

    void VulkanShader::bind()
    {

    }

    void VulkanShader::unbind()
    {

    }

    void VulkanShader::addStage(ShaderStage stage, const std::string& path)
    {
        const auto code = loadFile(path);
        const auto module = createShaderModule(code);
        m_shader_stages[stage] = module;
    }

    void VulkanShader::buildVertexShader(const VertexShader& shader_template)
    {
        NB_CORE_ASSERT(!shader_template.vertex_stage.empty() && !shader_template.fragment_stage.empty(), "Incomplete Vulkan VertexShader template!");

        addStage(ShaderStage::cVertex, shader_template.vertex_stage);
        addStage(ShaderStage::cFragment, shader_template.fragment_stage);
    }

    void* VulkanShader::getStageHandle(const ShaderStage stage)
    {
        if (!m_shader_stages.contains(stage))
            throw std::runtime_error(std::format("Vulkan shader {} does not contain {} stage!", getName(), shaderStageToString(stage)));

        return m_shader_stages[stage];
    }

    std::vector<char> VulkanShader::loadSourceFile(const std::string& path)
    {
        throw std::runtime_error("Vulkan shader does not support source files!");
    }

    VkShaderModule VulkanShader::createShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shader_module;
        const auto result = vkCreateShaderModule(VulkanAPI::getDevice(), &create_info, nullptr, &shader_module);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan shader module!");

        return shader_module;
    }

}
