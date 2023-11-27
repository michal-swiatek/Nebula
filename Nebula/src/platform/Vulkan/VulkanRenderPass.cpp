//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanRenderPass.h"

#include <vector>
#include <optional>

#include "platform/Vulkan/VulkanAPI.h"
#include "platform/Vulkan/VulkanAttachmentInfo.h"
#include "platform/Vulkan/VulkanTextureFormats.h"

struct AttachmentReferences
{
    std::vector<VkAttachmentReference> input_attachments;
    std::vector<VkAttachmentReference> color_attachments;
    std::optional<VkAttachmentReference> depth_stencil_attachment;
    std::vector<uint32_t> preserve_attachments;

    [[nodiscard]] VkSubpassDescription createSubpassDescription() const
    {
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        if (!input_attachments.empty())
        {
            subpass.inputAttachmentCount = input_attachments.size();
            subpass.pInputAttachments = input_attachments.data();
        }

        if (!color_attachments.empty())
        {
            subpass.colorAttachmentCount = color_attachments.size();
            subpass.pColorAttachments = color_attachments.data();
        }

        if (!preserve_attachments.empty())
        {
            subpass.preserveAttachmentCount = preserve_attachments.size();
            subpass.pPreserveAttachments = preserve_attachments.data();
        }

        if (depth_stencil_attachment)
            subpass.pDepthStencilAttachment = &*depth_stencil_attachment;

        return subpass;
    }
};

namespace nebula::rendering {

    AttachmentReferences createAttachmentReferences(const RenderStage& render_stage)
    {
        auto attachments = AttachmentReferences{};
        for (const auto& [index, layout, type] : render_stage.attachment_references)
        {
            switch (type)
            {
                case AttachmentReferenceType::cInput:
                    attachments.input_attachments.emplace_back(index, getVulkanAttachmentLayout(layout));     break;
                case AttachmentReferenceType::cColor:
                    attachments.color_attachments.emplace_back(index, getVulkanAttachmentLayout(layout));     break;
                case AttachmentReferenceType::cDepthStencil:
                    attachments.depth_stencil_attachment = {index, getVulkanAttachmentLayout(layout)};      break;
                case AttachmentReferenceType::cPreserve:
                    attachments.preserve_attachments.emplace_back(index);  break;
                default:    NB_CORE_ASSERT(false, "Resolve attachments are not supported!");
            }
        }

        return attachments;
    }

    VkAttachmentDescription createAttachmentDescription(const AttachmentDescription& attachment_description)
    {
        VkAttachmentDescription vulkan_attachment_description{};

        vulkan_attachment_description.format = getVulkanTextureFormat(attachment_description.format);
        vulkan_attachment_description.samples = getVulkanTextureSampling(attachment_description.samples);
        vulkan_attachment_description.loadOp = getVulkanAttachmentLoadOp(attachment_description.load_op);
        vulkan_attachment_description.storeOp = getVulkanAttachmentStoreOp(attachment_description.store_op);
        vulkan_attachment_description.stencilLoadOp = getVulkanAttachmentLoadOp(attachment_description.stencil_load_op);
        vulkan_attachment_description.stencilStoreOp = getVulkanAttachmentStoreOp(attachment_description.stencil_store_op);
        vulkan_attachment_description.initialLayout = getVulkanAttachmentLayout(attachment_description.initial_layout);
        vulkan_attachment_description.finalLayout = getVulkanAttachmentLayout(attachment_description.final_layout);

        return vulkan_attachment_description;
    }

    VulkanRenderPass::VulkanRenderPass(const Reference<RenderPassTemplate>& renderpass_template) :
        RenderPass(renderpass_template)
    {
        const auto& framebuffer_template = renderpass_template->viewFramebufferTemplate();

        const size_t n_subpasses = renderpass_template->viewRenderStages().size();
        size_t n_attachments = framebuffer_template->viewTextureAttachmentsDescriptions().size();
        if (framebuffer_template->viewDepthStencilAttachmentDescription())
            n_attachments++;

        //  Attachment references have to be stored in scope because VkSubpassDescription references them by pointer
        std::vector<AttachmentReferences> attachment_references(n_subpasses);
        std::vector<VkSubpassDescription> subpass_descriptions(n_subpasses);
        std::vector<VkAttachmentDescription> attachment_descriptions(n_attachments);

        for (const auto& render_stage : renderpass_template->viewRenderStages())
            attachment_references.emplace_back(createAttachmentReferences(render_stage));

        for (const auto& attachment_reference : attachment_references)
            subpass_descriptions.emplace_back(attachment_reference.createSubpassDescription());

        for (const auto& attachment_description : framebuffer_template->viewTextureAttachmentsDescriptions())
            attachment_descriptions.emplace_back(createAttachmentDescription(attachment_description));
        if (framebuffer_template->viewDepthStencilAttachmentDescription())
            attachment_descriptions.emplace_back(createAttachmentDescription(*framebuffer_template->viewDepthStencilAttachmentDescription()));

        VkRenderPassCreateInfo create_info{};

        create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        create_info.attachmentCount = attachment_descriptions.size();
        create_info.pAttachments = attachment_descriptions.data();
        create_info.subpassCount = subpass_descriptions.size();
        create_info.pSubpasses = subpass_descriptions.data();

        const auto result = vkCreateRenderPass(VulkanAPI::getDevice(), &create_info, nullptr, &m_renderpass);
        NB_CORE_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan RenderPass!");
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        vkDestroyRenderPass(VulkanAPI::getDevice(), m_renderpass, nullptr);
    }

    void* VulkanRenderPass::getRenderPassHandle()
    {
        return m_renderpass;
    }

}
