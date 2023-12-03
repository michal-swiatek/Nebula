//
// Created by michal-swiatek on 27.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef ATTACHMENTINFO_H
#define ATTACHMENTINFO_H

#include "core/Core.h"
#include "TextureFormats.h"

namespace nebula::rendering {

    enum class AttachmentOp
    {
        cClear,
        cStore,
        cLoad,
        cDontCare
    };

    enum class AttachmentLayout
    {
        cUndefined,
        cGeneral,
        cColorOptimal,
        cPresentOptimal,
        cDepthStencilOptimal,
        cDepthStencilReadOnlyOptimal,
        cShaderReadOnlyOptimal,
        cTransferSource,
        cTransferDestination
    };

    enum class AttachmentReferenceType
    {
        cInput,
        cColor,
        cDepthStencil,
        cResolve,
        cPreserve
    };

    struct NEBULA_API AttachmentDescription
    {
        TextureFormat format = TextureFormat::cFormat_R8G8B8A8_SRGB;
        TextureSampling samples = TextureSampling::cTexture1Sample;

        AttachmentLayout initial_layout = AttachmentLayout::cUndefined;
        AttachmentLayout final_layout = AttachmentLayout::cColorOptimal;

        AttachmentOp load_op = AttachmentOp::cClear;
        AttachmentOp store_op = AttachmentOp::cStore;

        AttachmentOp stencil_load_op = AttachmentOp::cDontCare;
        AttachmentOp stencil_store_op = AttachmentOp::cDontCare;

        auto operator <=> (const AttachmentDescription&) const = default;
    };

    struct NEBULA_API AttachmentReference
    {
        uint32_t index;
        AttachmentLayout layout = AttachmentLayout::cColorOptimal;
        AttachmentReferenceType type = AttachmentReferenceType::cColor;
    };

}

#endif //ATTACHMENTINFO_H
