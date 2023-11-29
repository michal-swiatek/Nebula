//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "renderer/RendererAPI.h"

#include "core/Types.h"
#include "core/Assert.h"

#include "renderer/RenderCommand.h"
#include "platform/Vulkan/VulkanRendererAPI.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace nebula::rendering {

    Scope<RendererApi> RendererApi::s_renderer_api = nullptr;
    View<RendererApi> RenderCommand::s_renderer_api = nullptr;

    void RendererApi::create(const API api)
    {
        NB_CORE_ASSERT(!s_renderer_api, "RendererAPI is already initialized!");
        NB_CORE_INFO("Initializing {} RendererAPI!", api == API::cVulkan ? "Vulkan" : "OpenGL");

        switch (api)
        {
            case API::cVulkan:  s_renderer_api = createScope<VulkanRendererApi>();    break;
            case API::cOpenGL:  s_renderer_api = createScope<OpenGlRendererApi>();    break;
            default:            NB_CORE_ASSERT(false, "Undefined Rendering API!");
        }

        RenderCommand::s_renderer_api = get();
    }

    void RendererApi::destroy()
    {
        NB_CORE_ASSERT(s_renderer_api, "RendererAPI is not initialized!");
        NB_CORE_INFO("Destroing RendererAPI!");

        s_renderer_api.reset(nullptr);
        RenderCommand::s_renderer_api = nullptr;
    }

    View<RendererApi> RendererApi::get()
    {
        NB_CORE_ASSERT(s_renderer_api, "RendererAPI is not initialized!");
        return s_renderer_api.get();
    }

}
