//
// Created by michal-swiatek on 23.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <optional>

#include "core/Core.h"
#include "core/Types.h"
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
        cShaderReadOnlyOptimal,
        cTransferSource,
        cTransferDestination
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
    };

    class FramebufferTemplate;

    class NEBULA_API Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        [[nodiscard]] virtual bool attached() const = 0;
        virtual void attachTo(void* renderpass_handle) = 0;

        [[nodiscard]] virtual const Reference<FramebufferTemplate>& getFramebufferTemplate() const = 0;
        [[nodiscard]] static Reference<Framebuffer> createFramebuffer(const Reference<FramebufferTemplate>& framebuffer_template);
    };

    class NEBULA_API FramebufferTemplate
    {
    public:
        FramebufferTemplate(uint32_t width, uint32_t height, uint32_t layers = 1);
        virtual ~FramebufferTemplate() = default;

        [[nodiscard]] const std::vector<AttachmentDescription>& viewTextureAttachmentsDescriptions() const;
        [[nodiscard]] const std::optional<AttachmentDescription>& viewDepthStencilAttachmentDescription() const;

        [[nodiscard]] uint32_t getWidth() const { return m_width; }
        [[nodiscard]] uint32_t getHeight() const { return m_height; }
        [[nodiscard]] uint32_t getLayers() const { return m_layers; }

    protected:
        void addTextureAttachment(const AttachmentDescription& attachment_description);
        void setDepthStencilAttachment(const std::optional<AttachmentDescription>& depth_stencil_attachment = std::nullopt);

    private:
        std::vector<AttachmentDescription> m_texture_attachments{};
        std::optional<AttachmentDescription> m_depth_stencil_attachment{};

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_layers;
    };

}

#endif //FRAMEBUFFER_H
