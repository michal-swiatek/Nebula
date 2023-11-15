//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanContext.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "core/Core.h"
#include "core/Logging.h"
#include "core/Application.h"

namespace nebula::rendering {

    VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo();
    VkApplicationInfo createApplicationInfo();
    bool checkValidationLayerSupport(const std::vector<const char*>& requested_layers);

    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    )
    {
        if (severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            return VK_FALSE;

        std::string message = callback_data->pMessage;

        switch (severity)
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:     NB_CORE_FATAL(message); break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:   NB_CORE_WARN(message);  break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:      NB_CORE_INFO(message);  break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:   NB_CORE_TRACE(message); break;
            default:                                                NB_CORE_ASSERT(false, "Unknown severity level!");
        }

        return VK_FALSE;
    }

    VkResult createDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    )
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        else
            return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

    void destroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator
    )
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
            func(instance, debugMessenger, pAllocator);
    }

    VulkanContext::VulkanContext(GLFWwindow* window_handle) : m_window(window_handle), m_instance()
    {
        NB_CORE_ASSERT(m_window, "Window handle in null!");

        uint32_t extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

        std::vector<VkExtensionProperties> supported_extensions(extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, supported_extensions.data());

        NB_CORE_INFO("Vulkan extensions:");
        for (const auto& extension : supported_extensions)
            NB_CORE_INFO("  {0}", extension.extensionName);

        VkApplicationInfo app_info = createApplicationInfo();

        uint32_t glfw_extension_count;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;

        #ifdef NB_DEBUG_BUILD
        const std::vector<const char*> validation_layers = {"VK_LAYER_KHRONOS_validation"};
        NB_CORE_ASSERT(checkValidationLayerSupport(validation_layers), "Vulkan validation layers are not available!");
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #else
        create_info.enabledLayerCount = 0;
        #endif

        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        VkResult status = vkCreateInstance(&create_info, nullptr, &m_instance);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to initialize Vulkan!");

        #ifdef NB_DEBUG_BUILD
        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = createDebugUtilsMessengerInfo();
        NB_CORE_ASSERT(createDebugUtilsMessengerEXT(m_instance, &debug_messenger_info, nullptr, &m_debug_messenger) == VK_SUCCESS, "Unable to set Vulkan debug callback!");
        NB_CORE_INFO("Using Vulkan validation layers");
        #endif
    }

    VulkanContext::~VulkanContext()
    {
        destroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

    void VulkanContext::bind()
    {
        // glfwMakeContextCurrent(m_window);
    }

    void VulkanContext::unbind()
    {
        // glfwMakeContextCurrent(nullptr);
    }

    void VulkanContext::swapBuffers()
    {
        // glfwSwapBuffers(m_window);
    }

    VkApplicationInfo createApplicationInfo()
    {
        VkApplicationInfo app_info{};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = Application::get().getName().c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Nebula";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_0;

        return app_info;
    }

    bool checkValidationLayerSupport(const std::vector<const char*>& requested_layers)
    {
        uint32_t layer_count;
        vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

        std::vector<VkLayerProperties> available_layers(layer_count);
        vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

        return true;    //  TODO: Implement check
    }

    VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo()
    {
        VkDebugUtilsMessengerCreateInfoEXT create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        create_info.pfnUserCallback = debugCallback;
        create_info.pUserData = nullptr;

        return create_info;
    }

}
