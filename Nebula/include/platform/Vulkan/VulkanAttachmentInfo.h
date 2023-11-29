//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANATTACHMENTINFO_H
#define VULKANATTACHMENTINFO_H

#include <vulkan/vulkan.h>

#include "rendering/AttachmentInfo.h"

namespace nebula::rendering {

    inline VkAttachmentLoadOp getVulkanAttachmentLoadOp(const AttachmentOp attachment_op)
    {
        switch (attachment_op)
        {
            case AttachmentOp::cClear:      return VK_ATTACHMENT_LOAD_OP_CLEAR;
            case AttachmentOp::cLoad:       return VK_ATTACHMENT_LOAD_OP_LOAD;
            case AttachmentOp::cDontCare:   return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            case AttachmentOp::cStore:      NB_CORE_ASSERT(false, "Store is not a load operation!");
        }

        return VK_ATTACHMENT_LOAD_OP_NONE_EXT;
    }

    inline VkAttachmentStoreOp getVulkanAttachmentStoreOp(const AttachmentOp attachment_op)
    {
        switch (attachment_op)
        {
            case AttachmentOp::cStore:      return VK_ATTACHMENT_STORE_OP_STORE;
            case AttachmentOp::cDontCare:   return VK_ATTACHMENT_STORE_OP_DONT_CARE;
            case AttachmentOp::cClear:
            case AttachmentOp::cLoad:       NB_CORE_ASSERT(false, "Clear/Load is not a store operation!");
        }

        return VK_ATTACHMENT_STORE_OP_NONE;
    }

    inline VkImageLayout getVulkanAttachmentLayout(const AttachmentLayout attachment_layout)
    {
        switch (attachment_layout)
        {
            case AttachmentLayout::cUndefined:                      return VK_IMAGE_LAYOUT_UNDEFINED;
            case AttachmentLayout::cGeneral:                        return VK_IMAGE_LAYOUT_GENERAL;
            case AttachmentLayout::cColorOptimal:                   return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            case AttachmentLayout::cPresentOptimal:                 return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            case AttachmentLayout::cDepthStencilOptimal:            return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            case AttachmentLayout::cDepthStencilReadOnlyOptimal:    return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            case AttachmentLayout::cShaderReadOnlyOptimal:          return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            case AttachmentLayout::cTransferSource:                 return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            case AttachmentLayout::cTransferDestination:            return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        }

        return VK_IMAGE_LAYOUT_UNDEFINED;
    }

}

#endif //VULKANATTACHMENTINFO_H
