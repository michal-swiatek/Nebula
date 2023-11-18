//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANCONTEXT_H
#define VULKANCONTEXT_H

#include "renderer/RenderContext.h"

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace nebula::rendering {

    class VulkanContext final : public RenderContext
    {
    public:
        explicit VulkanContext(GLFWwindow* window_handle);
        ~VulkanContext() override;

        void bind() override;
        void unbind() override;

        void swapBuffers() override;

    private:
        GLFWwindow* m_window;
        VkInstance m_instance;

        #ifdef NB_DEBUG_BUILD
        VkDebugUtilsMessengerEXT m_debug_messenger;
        #endif
    };

}

#endif //VULKANCONTEXT_H
