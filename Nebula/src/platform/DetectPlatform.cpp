//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/DetectPlatform.h"

#include "core/Config.h"
#include "core/Application.h"

#include "core/Window.h"
#include "core/Input.h"
#include "core/Timer.h"
#include "core/Types.h"

#include "rendering/renderer/RendererAPI.h"
#include "rendering/renderpass/RenderPassExecutor.h"

#include "platform/Vulkan/VulkanShader.h"
#include "platform/Vulkan/VulkanContext.h"
#include "platform/Vulkan/VulkanRenderPass.h"
#include "platform/Vulkan/VulkanFramebuffer.h"
#include "platform/Vulkan/VulkanImGuiBackend.h"
#include "platform/Vulkan/VulkanRenderPassExecutor.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLContext.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"
#include "platform/OpenGL/OpenGLImGuiBackend.h"

#ifdef NB_PLATFORM_WINDOWS
    #include <Windows.h>
    #include "platform/Windows/WindowsWindow.h"
    #include "platform/Windows/WindowsInput.h"
#endif

using namespace nebula::rendering;

namespace nebula {

    Input* Input::s_instance = nullptr;

    Scope<Window> Window::create(const WindowProperties& properties, const rendering::API api)
    {
        #ifdef NB_PLATFORM_WINDOWS
        return createScope<WindowsWindow>(properties, api);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }

    Scope<Input> Input::create(View<Window> window)
    {
        #ifdef NB_PLATFORM_WINDOWS
        return createScope<WindowsInput>(*window);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
        #endif
    }

    Scope<ImGuiBackend> ImGuiBackend::create(RenderPass& renderpass)
    {
        ImGuiBackend* backend = nullptr;
        switch (Application::get().getRenderingAPI())
        {
            case API::cVulkan:  backend = new VulkanImGuiBackend(renderpass);  break;
            case API::cOpenGL:  backend = new OpenGlImGuiBackend(renderpass);  break;
            default:    NB_CORE_ASSERT(false, "Unsupported rendering API!");
        }

        return createScopeFromPointer(backend);
    }

    namespace rendering {

        Scope<RenderContext> RenderContext::create(void* window_handle)
        {
            RenderContext* render_context = nullptr;
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    render_context = new OpenGLContext(static_cast<GLFWwindow*>(window_handle));  break;
                case API::cVulkan:    render_context = new VulkanContext(static_cast<GLFWwindow*>(window_handle));  break;
                default:                      NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }

            return createScopeFromPointer(render_context);
        }

        Reference<Shader> Shader::create(const std::string& name, const ShaderTemplate& shader_template)
        {
            Shader* shader = nullptr;
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    shader = new OpenGLShader(name, shader_template);  break;
                case API::cVulkan:    shader = new VulkanShader(name, shader_template);  break;
                default:              NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }

            return createReferenceFromPointer(shader);
        }

        Scope<RenderPass> RenderPass::create(const Reference<RenderPassTemplate>& renderpass_template, const bool create_framebuffer)
        {
            NB_CORE_ASSERT(renderpass_template->viewFramebufferTemplate(), "Cannot create RenderPass without FramebufferTemplate!");
            NB_CORE_ASSERT(!renderpass_template->viewRenderStages().empty(), "RenderPass has to have at least one RenderStage!");

            RenderPass* renderpass = nullptr;
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    renderpass = new RenderPass(renderpass_template, create_framebuffer);        break;
                case API::cVulkan:    renderpass = new VulkanRenderPass(renderpass_template, create_framebuffer);  break;
                default:    NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }

            RendererApi::get().compilePipelines(*renderpass);

            return createScopeFromPointer(renderpass);
        }

        Scope<RenderPassExecutor> RenderPassExecutor::create(Scope<Renderer>&& renderer)
        {
            RenderPassExecutor* renderpass_executor = nullptr;
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    renderpass_executor = new RenderPassExecutor(std::move(renderer)); break;
                case API::cVulkan:    renderpass_executor = new VulkanRenderPassExecutor(std::move(renderer));  break;
                default:    NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }

            return createScopeFromPointer(renderpass_executor);
        }

        Scope<RenderPassExecutor> RenderPassExecutor::create(Scope<RenderPass>&& renderpass)
        {
            RenderPassExecutor* renderpass_executor = nullptr;
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    renderpass_executor = new RenderPassExecutor(std::move(renderpass)); break;
                case API::cVulkan:    renderpass_executor = new VulkanRenderPassExecutor(std::move(renderpass));    break;
                default:    NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }

            return createScopeFromPointer(renderpass_executor);
        }

        Reference<Framebuffer> Framebuffer::create(const Reference<FramebufferTemplate>& framebuffer_template)
        {
            Framebuffer* framebuffer = nullptr;
            switch (Application::get().getRenderingAPI())
            {
                case API::cVulkan:  framebuffer = new VulkanFramebuffer(framebuffer_template);  break;
                case API::cOpenGL:  framebuffer = new OpenGlFramebuffer(framebuffer_template);  break;
                default:    NB_CORE_ASSERT(false, "Unknown rendering API!");
            }

            return createReferenceFromPointer(framebuffer);
        }

    }

    void system_sleep(double seconds)
    {
        const auto nanoseconds = static_cast<long long>(seconds * 1'000'000'000);

        #ifdef NB_PLATFORM_WINDOWS
        HANDLE timer = CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_MANUAL_RESET | CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_MODIFY_STATE | SYNCHRONIZE);
        NB_CORE_ASSERT(timer, "Unable to create Windows high resolution timer!");

        LARGE_INTEGER li;
        li.QuadPart = -nanoseconds / 100;    //  Windows expects 100 nanosecond intervals
        bool result = SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE);
        NB_CORE_ASSERT(result, "Unable to set Windows high resolution timer delay!");

        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);
        #else
        NB_CORE_ASSERT(false, "Unknown platform!");
        #endif
    }

}
