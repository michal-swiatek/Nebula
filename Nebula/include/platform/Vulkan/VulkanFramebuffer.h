//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANFRAMEBUFFER_H
#define VULKANFRAMEBUFFER_H

#include "rendering/Framebuffer.h"

#include "core/Types.h"
#include "platform/Vulkan/VulkanAPI.h"

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

        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const override;

    private:
        VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
        Reference<FramebufferTemplate> m_framebuffer_template = nullptr;

        std::vector<VkImageView> m_image_views{};
        std::vector<VkApiAllocatedImage> m_image_buffers{};

        void createAttachment(const AttachmentDescription& attachment_description, bool depth_stencil);
    };

    class SwapchainFramebufferTemplate final : public FramebufferTemplate
    {
    public:
        SwapchainFramebufferTemplate(
            const uint32_t width,
            const uint32_t height,
            VkFormat format
        ) :
                FramebufferTemplate(width, height)
        {
            //  TODO: Convert Vulkan format to Nebula format
            AttachmentDescription attachment_description;
            addTextureAttachment(attachment_description);
        }
    };

    class VulkanSwapchain;
    class VulkanSwapchainImages;

    class VulkanSwapchainFramebuffer final : public Framebuffer
    {
    public:
        explicit VulkanSwapchainFramebuffer(VkImageView image_view, const Reference<FramebufferTemplate>& swapchain_framebuffer_template);
        ~VulkanSwapchainFramebuffer() override;

        void bind() override;
        void unbind() override;

        [[nodiscard]] bool attached() const override;
        void attachTo(void* renderpass_handle) override;

        [[nodiscard]] const Reference<FramebufferTemplate>& viewFramebufferTemplate() const override;

    private:
        VkImageView m_image_view = VK_NULL_HANDLE;
        VkFramebuffer m_framebuffer = VK_NULL_HANDLE;

        Reference<FramebufferTemplate> m_framebuffer_template;

        //  For swapchain template access
        friend class VulkanSwapchain;
        friend class VulkanSwapchainImages;
    };

}

#endif //VULKANFRAMEBUFFER_H
