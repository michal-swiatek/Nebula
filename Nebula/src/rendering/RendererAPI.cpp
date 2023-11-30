//
// Created by michal-swiatek on 29.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/renderer/RendererAPI.h"

#include "core/Types.h"
#include "core/Assert.h"

#include "platform/Vulkan/VulkanRendererAPI.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

namespace nebula::rendering {

    Scope<RendererApi> RendererApi::s_renderer_api = nullptr;

    void RendererApi::create(const API api)
    {
        NB_CORE_ASSERT(!s_renderer_api, "RendererAPI is already initialized!");
        NB_CORE_INFO("Initializing {} RendererAPI!", api == API::cVulkan ? "Vulkan" : "OpenGL");

        RendererApi* renderer_api = nullptr;
        switch (api)
        {
            case API::cVulkan:  renderer_api = new VulkanRendererApi();    break;
            case API::cOpenGL:  renderer_api = new OpenGlRendererApi();    break;
            default:            NB_CORE_ASSERT(false, "Undefined Rendering API!");
        }

        s_renderer_api = createScopeFromPointer(renderer_api);
    }

    void RendererApi::destroy()
    {
        NB_CORE_ASSERT(s_renderer_api, "RendererAPI is not initialized!");
        NB_CORE_INFO("Destroing RendererAPI!");

        s_renderer_api.reset(nullptr);
    }

    View<RendererApi> RendererApi::get()
    {
        NB_CORE_ASSERT(s_renderer_api, "RendererAPI is not initialized!");
        return s_renderer_api.get();
    }

}
