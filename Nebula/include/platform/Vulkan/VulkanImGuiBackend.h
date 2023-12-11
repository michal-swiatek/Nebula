//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANIMGUIBACKEND_H
#define VULKANIMGUIBACKEND_H

#include "debug/ImGuiBackend.h"
#include "platform/Vulkan/VulkanAPI.h"
#include "platform/Vulkan/VulkanRecordedBuffer.h"

namespace nebula {

    class VulkanImGuiBackend final : public ImGuiBackend
    {
    public:
        explicit VulkanImGuiBackend(rendering::RenderPass& renderpass);

        void onAttach() override;
        void onDetach() override;

        void begin() override;
        void end(void* command_buffer_handle) override;

    private:
        rendering::RenderPass& m_renderpass;

        static VkDescriptorPool s_descriptor_pool;

        static void init();
        static void shutdown();
        friend nebula::ImGuiBackend;
    };

}

#endif //VULKANIMGUIBACKEND_H
