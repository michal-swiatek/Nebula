//
// Created by michal-swiatek on 24.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANTEXTUREFORMATS_H
#define VULKANTEXTUREFORMATS_H

#include "rendering/TextureFormats.h"

#include <vulkan/vulkan.h>

namespace nebula::rendering {

    inline VkFormat getVulkanTextureFormat(const TextureFormat texture_format)
    {
        switch (texture_format)
        {
            case TextureFormat::cUndefined:                         return VK_FORMAT_UNDEFINED;

            case TextureFormat::cFormat_R8_UINT:                    return VK_FORMAT_R8_UINT;
            case TextureFormat::cFormat_R8_SINT:                    return VK_FORMAT_R8_SINT;
            case TextureFormat::cFormat_R8_SRGB:                    return VK_FORMAT_R8_SRGB;
            case TextureFormat::cFormat_R8_FLOAT:                   return VK_FORMAT_R8_USCALED;
            case TextureFormat::cFormat_R8_FLOAT_NORM:              return VK_FORMAT_R8_UNORM;

            case TextureFormat::cFormat_R16_UINT:                   return VK_FORMAT_R16_UINT;
            case TextureFormat::cFormat_R16_SINT:                   return VK_FORMAT_R16_SINT;
            case TextureFormat::cFormat_R16_FLOAT:                  return VK_FORMAT_R16_USCALED;
            case TextureFormat::cFormat_R16_FLOAT_NORM:             return VK_FORMAT_R16_UNORM;

            case TextureFormat::cFormat_R32_UINT:                   return VK_FORMAT_R32_UINT;
            case TextureFormat::cFormat_R32_SINT:                   return VK_FORMAT_R32_SINT;
            case TextureFormat::cFormat_R32_FLOAT:                  return VK_FORMAT_R32_SFLOAT;

            case TextureFormat::cFormat_R64_UINT:                   return VK_FORMAT_R64_UINT;
            case TextureFormat::cFormat_R64_SINT:                   return VK_FORMAT_R64_SINT;
            case TextureFormat::cFormat_R64_FLOAT:                  return VK_FORMAT_R64_SFLOAT;

            case TextureFormat::cFormat_R8G8_UINT:                  return VK_FORMAT_R8G8_UINT;
            case TextureFormat::cFormat_R8G8_SINT:                  return VK_FORMAT_R8G8_SINT;
            case TextureFormat::cFormat_R8G8_SRGB:                  return VK_FORMAT_R8G8_SRGB;
            case TextureFormat::cFormat_R8G8_FLOAT:                 return VK_FORMAT_R8G8_USCALED;
            case TextureFormat::cFormat_R8G8_FLOAT_NORM:            return VK_FORMAT_R8G8_UNORM;

            case TextureFormat::cFormat_R16G16_UINT:                return VK_FORMAT_R16G16_UINT;
            case TextureFormat::cFormat_R16G16_SINT:                return VK_FORMAT_R16G16_SINT;
            case TextureFormat::cFormat_R16G16_FLOAT:               return VK_FORMAT_R16G16_USCALED;
            case TextureFormat::cFormat_R16G16_FLOAT_NORM:          return VK_FORMAT_R16G16_UNORM;

            case TextureFormat::cFormat_R32G32_UINT:                return VK_FORMAT_R32G32_UINT;
            case TextureFormat::cFormat_R32G32_SINT:                return VK_FORMAT_R32G32_SINT;
            case TextureFormat::cFormat_R32G32_FLOAT:               return VK_FORMAT_R32G32_SFLOAT;

            case TextureFormat::cFormat_R64G64_UINT:                return VK_FORMAT_R64G64_UINT;
            case TextureFormat::cFormat_R64G64_SINT:                return VK_FORMAT_R64G64_SINT;
            case TextureFormat::cFormat_R64G64_FLOAT:               return VK_FORMAT_R64G64_SFLOAT;

            case TextureFormat::cFormat_R8G8B8_UINT:                return VK_FORMAT_R8G8B8_UINT;
            case TextureFormat::cFormat_R8G8B8_SINT:                return VK_FORMAT_R8G8B8_SINT;
            case TextureFormat::cFormat_R8G8B8_SRGB:                return VK_FORMAT_R8G8B8_SRGB;
            case TextureFormat::cFormat_R8G8B8_FLOAT:               return VK_FORMAT_R8G8B8_USCALED;
            case TextureFormat::cFormat_R8G8B8_FLOAT_NORM:          return VK_FORMAT_R8G8B8_UNORM;

            case TextureFormat::cFormat_R16G16B16_UINT:             return VK_FORMAT_R16G16B16_UINT;
            case TextureFormat::cFormat_R16G16B16_SINT:             return VK_FORMAT_R16G16B16_SINT;
            case TextureFormat::cFormat_R16G16B16_FLOAT:            return VK_FORMAT_R16G16B16_USCALED;
            case TextureFormat::cFormat_R16G16B16_FLOAT_NORM:       return VK_FORMAT_R16G16B16_UNORM;

            case TextureFormat::cFormat_R32G32B32_UINT:             return VK_FORMAT_R32G32B32_UINT;
            case TextureFormat::cFormat_R32G32B32_SINT:             return VK_FORMAT_R32G32B32_SINT;
            case TextureFormat::cFormat_R32G32B32_FLOAT:            return VK_FORMAT_R32G32B32_SFLOAT;

            case TextureFormat::cFormat_R64G64B64_UINT:             return VK_FORMAT_R64G64B64_UINT;
            case TextureFormat::cFormat_R64G64B64_SINT:             return VK_FORMAT_R64G64B64_SINT;
            case TextureFormat::cFormat_R64G64B64_FLOAT:            return VK_FORMAT_R64G64B64_SFLOAT;

            case TextureFormat::cFormat_R8G8B8A8_UINT:              return VK_FORMAT_R8G8B8A8_UINT;
            case TextureFormat::cFormat_R8G8B8A8_SINT:              return VK_FORMAT_R8G8B8A8_SINT;
            case TextureFormat::cFormat_R8G8B8A8_SRGB:              return VK_FORMAT_R8G8B8A8_SRGB;
            case TextureFormat::cFormat_R8G8B8A8_FLOAT:             return VK_FORMAT_R8G8B8A8_USCALED;
            case TextureFormat::cFormat_R8G8B8A8_FLOAT_NORM:        return VK_FORMAT_R8G8B8A8_UNORM;

            case TextureFormat::cFormat_R16G16B16A16_UINT:          return VK_FORMAT_R16G16B16A16_UINT;
            case TextureFormat::cFormat_R16G16B16A16_SINT:          return VK_FORMAT_R16G16B16A16_SINT;
            case TextureFormat::cFormat_R16G16B16A16_FLOAT:         return VK_FORMAT_R16G16B16A16_USCALED;
            case TextureFormat::cFormat_R16G16B16A16_FLOAT_NORM:    return VK_FORMAT_R16G16B16A16_UNORM;

            case TextureFormat::cFormat_R32G32B32A32_UINT:          return VK_FORMAT_R32G32B32A32_UINT;
            case TextureFormat::cFormat_R32G32B32A32_SINT:          return VK_FORMAT_R32G32B32A32_SINT;
            case TextureFormat::cFormat_R32G32B32A32_FLOAT:         return VK_FORMAT_R32G32B32A32_SFLOAT;

            case TextureFormat::cFormat_R64G64B64A64_UINT:          return VK_FORMAT_R64G64B64A64_UINT;
            case TextureFormat::cFormat_R64G64B64A64_SINT:          return VK_FORMAT_R64G64B64A64_SINT;
            case TextureFormat::cFormat_R64G64B64A64_FLOAT:         return VK_FORMAT_R64G64B64A64_SFLOAT;

            case TextureFormat::cFormat_S8_UINT:                    return VK_FORMAT_S8_UINT;
            case TextureFormat::cFormat_D16_UNROM:                  return VK_FORMAT_D16_UNORM;
            case TextureFormat::cFormat_D32_SFLOAT:                 return VK_FORMAT_D32_SFLOAT;
            case TextureFormat::cFormat_D16_UNROM_S8_UINT:          return VK_FORMAT_D16_UNORM_S8_UINT;
            case TextureFormat::cFormat_D24_UNROM_S8_UINT:          return VK_FORMAT_D24_UNORM_S8_UINT;
            case TextureFormat::cFormat_D32_SFLOAT_S8_UINT:         return VK_FORMAT_D32_SFLOAT_S8_UINT;
        }

        return VK_FORMAT_UNDEFINED;
    }

    inline VkSampleCountFlagBits getVulkanTextureSampling(const TextureSampling texture_sampling)
    {
        switch (texture_sampling)
        {
            case TextureSampling::cTexture1Sample:      return VK_SAMPLE_COUNT_1_BIT;
            case TextureSampling::cTexture2Samples:     return VK_SAMPLE_COUNT_2_BIT;
            case TextureSampling::cTexture4Samples:     return VK_SAMPLE_COUNT_4_BIT;
            case TextureSampling::cTexture8Samples:     return VK_SAMPLE_COUNT_8_BIT;
            case TextureSampling::cTexture16Samples:    return VK_SAMPLE_COUNT_16_BIT;
            case TextureSampling::cTexture32Samples:    return VK_SAMPLE_COUNT_32_BIT;
            case TextureSampling::cTexture64Samples:    return VK_SAMPLE_COUNT_64_BIT;
        }

        return VK_SAMPLE_COUNT_1_BIT;
    }

}

#endif //VULKANTEXTUREFORMATS_H
