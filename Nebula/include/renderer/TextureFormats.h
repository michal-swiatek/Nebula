//
// Created by michal-swiatek on 23.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef TEXTUREFORMATS_H
#define TEXTUREFORMATS_H

namespace nebula::rendering {

    enum class TextureFormat
    {
        cUndefined,

        //  1 channel formats
        cFormat_R8_UINT,        //  uint8
        cFormat_R8_SINT,        //  int8
        cFormat_R8_SRGB,        //  [0.0, 1.0] float in srgb color space
        cFormat_R8_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R8_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R16_UINT,        //  uint16
        cFormat_R16_SINT,        //  int16
        cFormat_R16_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R16_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R32_UINT,        //  uint32
        cFormat_R32_SINT,        //  int32
        cFormat_R32_FLOAT,       //  float32

        cFormat_R64_UINT,        //  uint64
        cFormat_R64_SINT,        //  int64
        cFormat_R64_FLOAT,       //  float64

        //  2 channel formats
        cFormat_R8G8_UINT,        //  uint8
        cFormat_R8G8_SINT,        //  int8
        cFormat_R8G8_SRGB,        //  [0.0, 1.0] float in srgb color space
        cFormat_R8G8_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R8G8_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R16G16_UINT,        //  uint16
        cFormat_R16G16_SINT,        //  int16
        cFormat_R16G16_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R16G16_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R32G32_UINT,        //  uint32
        cFormat_R32G32_SINT,        //  int32
        cFormat_R32G32_FLOAT,       //  float32

        cFormat_R64G64_UINT,        //  uint64
        cFormat_R64G64_SINT,        //  int64
        cFormat_R64G64_FLOAT,       //  float64

        //  3 channel formats
        cFormat_R8G8B8_UINT,        //  uint8
        cFormat_R8G8B8_SINT,        //  int8
        cFormat_R8G8B8_SRGB,        //  [0.0, 1.0] float in srgb color space
        cFormat_R8G8B8_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R8G8B8_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R16G16B16_UINT,        //  uint16
        cFormat_R16G16B16_SINT,        //  int16
        cFormat_R16G16B16_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R16G16B16_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R32G32B32_UINT,        //  uint32
        cFormat_R32G32B32_SINT,        //  int32
        cFormat_R32G32B32_FLOAT,       //  float32

        cFormat_R64G64B64_UINT,        //  uint64
        cFormat_R64G64B64_SINT,        //  int64
        cFormat_R64G64B64_FLOAT,       //  float64

        //  4 channel formats
        cFormat_R8G8B8A8_UINT,        //  uint8
        cFormat_R8G8B8A8_SINT,        //  int8
        cFormat_R8G8B8A8_SRGB,        //  [0.0, 1.0] float in srgb color space
        cFormat_R8G8B8A8_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R8G8B8A8_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R16G16B16A16_UINT,        //  uint16
        cFormat_R16G16B16A16_SINT,        //  int16
        cFormat_R16G16B16A16_FLOAT,       //  [0.0, 255.0] float in linear space
        cFormat_R16G16B16A16_FLOAT_NORM,  //  [0.0, 1.0] float in linear space

        cFormat_R32G32B32A32_UINT,        //  uint32
        cFormat_R32G32B32A32_SINT,        //  int32
        cFormat_R32G32B32A32_FLOAT,       //  float32

        cFormat_R64G64B64A64_UINT,        //  uint64
        cFormat_R64G64B64A64_SINT,        //  int64
        cFormat_R64G64B64A64_FLOAT,       //  float64

        //  Depth Stencil
        cFormat_S8_UINT,                //  256 values for stencil
        cFormat_D16_UNROM,              //  [0.0, 1.0] float16 depth
        cFormat_D32_SFLOAT,             //  float32 depth
        cFormat_D16_UNROM_S8_UINT,      //  [0.0, 1.0] float16 depth + 256 values for stencil
        cFormat_D24_UNROM_S8_UINT,      //  [0.0, 1.0] float24 depth + 256 values for stencil
        cFormat_D32_SFLOAT_S8_UINT,     //  float32 depth + 256 values for stencil
    };

    enum class TextureSampling
    {
        cTexture1Sample = 1,
        cTexture2Samples = 2,
        cTexture4Samples = 4,
        cTexture8Samples = 8,
        cTexture16Samples = 16,
        cTexture32Samples = 32,
        cTexture64Samples = 64
    };

    inline bool formatHasDepth(const TextureFormat texture_format)
    {
        return texture_format > TextureFormat::cFormat_R64G64B64A64_FLOAT && texture_format != TextureFormat::cFormat_S8_UINT;
    }

    inline bool formatHasStencil(const TextureFormat texture_format)
    {
        return texture_format > TextureFormat::cFormat_R64G64B64A64_FLOAT &&
        (
            texture_format == TextureFormat::cFormat_S8_UINT ||
            texture_format == TextureFormat::cFormat_D16_UNROM_S8_UINT ||
            texture_format == TextureFormat::cFormat_D24_UNROM_S8_UINT ||
            texture_format == TextureFormat::cFormat_D32_SFLOAT_S8_UINT
        );
    }

}

#endif //TEXTUREFORMATS_H
