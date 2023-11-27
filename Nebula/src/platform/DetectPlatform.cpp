//
// Created by michal-swiatek on 10.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/DetectPlatform.h"

#include "core/Config.h"
#include "core/Application.h"

#include "memory/MemoryManager.h"

#include "core/Window.h"
#include "core/Input.h"
#include "core/Timer.h"
#include "core/Types.h"

#include "renderer/RendererAPI.h"

#include "platform/Vulkan/VulkanContext.h"
#include "platform/Vulkan/VulkanRenderPass.h"
#include "platform/Vulkan/VulkanCommandPool.h"
#include "platform/Vulkan/VulkanFramebuffer.h"
#include "platform/Vulkan/VulkanRendererAPI.h"

#include "platform/OpenGL/OpenGLContext.h"
#include "platform/OpenGL/OpenGLFramebuffer.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

#ifdef NB_PLATFORM_WINDOWS
    #include <Windows.h>
    #include "platform/Windows/WindowsWindow.h"
    #include "platform/Windows/WindowsInput.h"
#endif

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

    namespace rendering {

        Scope<RenderContext> RenderContext::create(void* window_handle)
        {
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window_handle));
                case API::cVulkan:    return createScope<VulkanContext>(static_cast<GLFWwindow*>(window_handle));
                default:                      NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }
        }

        Scope<RenderCommandPool> RenderCommandPool::create()
        {
            auto& config = Config::getEngineConfig();
            const auto command_buffer_size = config["memory"]["event_queue_size"].as<size_t>();

            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    return createScope<RenderCommandPool>(command_buffer_size);
                case API::cVulkan:    return createScope<VulkanCommandPool>(command_buffer_size);
                default:    NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }
        }

        Scope<RenderPass> RenderPass::create(const Reference<RenderPassTemplate>& renderpass_template)
        {
            switch (Application::get().getRenderingAPI())
            {
                case API::cOpenGL:    return createScope<RenderPass>(renderpass_template);
                case API::cVulkan:    return createScope<VulkanRenderPass>(renderpass_template);
                default:    NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }
        }

        Reference<Framebuffer> Framebuffer::create(const Reference<FramebufferTemplate>& framebuffer_template)
        {
            switch (Application::get().getRenderingAPI())
            {
                case API::cVulkan:  return createReference<VulkanFramebuffer>(framebuffer_template);
                case API::cOpenGL:  return createReference<OpenGlFramebuffer>(framebuffer_template);
                default:    NB_CORE_ASSERT(false, "Unknown rendering API!");
            }

            return nullptr;
        }

        View<RendererApi> RendererApi::create(const API api)
        {
            switch (api)
            {
                case API::cOpenGL:    return memory::MemoryManager::create<OpenGlRendererApi>();
                case API::cVulkan:    return memory::MemoryManager::create<VulkanRendererApi>();
                default:     NB_CORE_ASSERT(false, "Undefined Rendering API!");  return nullptr;
            }
        }

        void RendererApi::destroy(RendererApi* api)
        {
            memory::MemoryManager::destroy(api);
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
