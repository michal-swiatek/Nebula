//
// Created by michal-swiatek on 30.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANSHADER_H
#define VULKANSHADER_H

#include "rendering/Shader.h"

#include <unordered_map>
#include "platform/Vulkan/VulkanAPI.h"

namespace nebula::rendering {

    class VulkanShader final : public Shader
    {
    public:
        VulkanShader(const std::string& name, const ShaderTemplate& shader_template);
        ~VulkanShader() override;

        void bind() override;
        void unbind() override;
        void* getStageHandle(ShaderStage stage) override;

    private:
        std::unordered_map<ShaderStage, VkShaderModule> m_shader_stages{};

        void addStage(ShaderStage stage, const std::string& path);
        void buildVertexShader(const VertexShader& shader_template);
        static VkShaderModule createShaderModule(const std::vector<char>& code);

        std::vector<char> loadSourceFile(const std::string& path) override;
    };

}

#endif //VULKANSHADER_H
