//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANFRAMEBUFFER_H
#define VULKANFRAMEBUFFER_H

#include "renderer/Framebuffer.h"

#include "platform/Vulkan/VulkanAPI.h"
#include "core/Types.h"

namespace nebula::rendering {

    class NEBULA_API VulkanFramebuffer final : public Framebuffer
    {
    public:
        explicit VulkanFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template);
        ~VulkanFramebuffer() override;

        void bind() override;
        void unbind() override;

        [[nodiscard]] bool attached() const override;
        void attachTo(void* renderpass_handle) override;

        [[nodiscard]] const Reference<FramebufferTemplate>& getFramebufferTemplate() const override;

    private:
        VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
        Reference<FramebufferTemplate> m_framebuffer_template = nullptr;

        std::vector<VkImageView> m_image_views{};
        std::vector<VkApiAllocatedImage> m_image_buffers{};

        void createAttachment(const AttachmentDescription& attachment_description, bool depth_stencil);
    };

    class VulkanSwapchainFramebuffers final : public Framebuffer
    {
    public:
        VulkanSwapchainFramebuffers(VkSwapchainKHR swapchain, VkSurfaceFormatKHR surface_format, uint32_t width, uint32_t height);
        ~VulkanSwapchainFramebuffers() override;

        void bind() override;
        void unbind() override;

        [[nodiscard]] bool attached() const override;
        void attachTo(void* renderpass_handle) override;

        [[nodiscard]] const Reference<FramebufferTemplate>& getFramebufferTemplate() const override;

    private:
        uint32_t m_width;
        uint32_t m_height;
        VkSurfaceFormatKHR m_surface_format;

        std::vector<VkImage> m_swapchain_images{};
        std::vector<VkImageView> m_swapchain_image_views{};
        std::vector<VkFramebuffer> m_swapchain_framebuffers{};
    };

}

#endif //VULKANFRAMEBUFFER_H
