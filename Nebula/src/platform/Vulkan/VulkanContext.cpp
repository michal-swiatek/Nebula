//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanContext.h"

#include <map>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "core/Assert.h"
#include "core/Logging.h"
#include "core/Application.h"

#include "platform/EngineConfiguration.h"
#include "platform/Vulkan/VulkanConfiguration.h"

struct DeviceProperties
{
    std::string vendor;
    std::string renderer;
    std::string driver_version;
    std::string api_version;
};

VkApplicationInfo createApplicationInfo();
VkDeviceQueueCreateInfo createQueueInfo(uint32_t queue_family_index, float priority = 1.0);

int ratePhysicalDevice(VkPhysicalDevice device);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance);
std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);
std::vector<VkExtensionProperties> getSupportedExtensions();

DeviceProperties getDeviceProperties(VkPhysicalDevice);

#ifdef NB_DEBUG_BUILD
bool checkValidationLayerSupport(const std::vector<const char*>& requested_layers);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void* user_data);

VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo();
VkResult createDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);
void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);
#endif

namespace nebula::rendering {

    VulkanContext::VulkanContext(GLFWwindow* window_handle) : m_window(window_handle)
    {
        NB_CORE_ASSERT(m_window, "Window handle in null!");

        createVulkanInstance();
        createSurface();
        pickPhysicalDevice();
        createLogicalDevice();

        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= 1)
        {
            auto device_properties = getDeviceProperties(m_physical_device);
            NB_CORE_INFO("Vulkan Info:");
            NB_CORE_INFO("  Vendor: {0}", device_properties.vendor);
            NB_CORE_INFO("  Renderer: {0}", device_properties.renderer);
            NB_CORE_INFO("  Vulkan version: {0}", device_properties.api_version);
            NB_CORE_INFO("  Driver version: {0}", device_properties.driver_version);
        }
        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= 2)
        {
            auto supported_extensions = getSupportedExtensions();
            NB_CORE_INFO("Vulkan extensions:");
            for (const auto& [extension_name, spec_version] : supported_extensions)
                NB_CORE_INFO("  {0}", extension_name);
        }
    }

    VulkanContext::~VulkanContext()
    {
        #ifdef NB_DEBUG_BUILD
        destroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
        #endif

        vkDestroyDevice(m_device, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
        vkDestroyInstance(m_instance, nullptr);
    }

    void VulkanContext::bind()
    {

    }

    void VulkanContext::unbind()
    {

    }

    void VulkanContext::swapBuffers()
    {

    }

    void VulkanContext::createVulkanInstance()
    {
        const VkApplicationInfo app_info = createApplicationInfo();

        uint32_t glfw_extension_count;
        const char** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

        VkInstanceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.pApplicationInfo = &app_info;

        #ifdef NB_DEBUG_BUILD
        const std::vector validation_layers = {"VK_LAYER_KHRONOS_validation"};
        NB_CORE_ASSERT(checkValidationLayerSupport(validation_layers), "Vulkan validation layers are not available!");
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        #else
        create_info.enabledLayerCount = 0;
        #endif

        create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        create_info.ppEnabledExtensionNames = extensions.data();

        const VkResult status = vkCreateInstance(&create_info, nullptr, &m_instance);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to initialize Vulkan!");

        #ifdef NB_DEBUG_BUILD
        VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = createDebugUtilsMessengerInfo();
        NB_CORE_ASSERT(
            createDebugUtilsMessengerEXT(m_instance, &debug_messenger_info, nullptr, &m_debug_messenger) == VK_SUCCESS,
            "Unable to set Vulkan debug callback!"
        );
        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= 1)
            NB_CORE_INFO("Using Vulkan validation layers");
        #endif
    }

    void VulkanContext::pickPhysicalDevice()
    {
        const auto devices = getPhysicalDevices(m_instance);
        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices)
        {
            if (auto queue_indices = findQueueFamilies(device); queue_indices.checkMinimalSupport())
                candidates.insert(std::make_pair(ratePhysicalDevice(device), device));
        }

        NB_CORE_ASSERT(candidates.rbegin()->first > 0, "Failed to find a suitable GPU!")
        m_physical_device = candidates.rbegin()->second;
        m_queue_family_indices = findQueueFamilies(m_physical_device);
    }

    void VulkanContext::createLogicalDevice()
    {
        VkDeviceQueueCreateInfo create_queue_info = createQueueInfo(*m_queue_family_indices.graphics_family);
        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = &create_queue_info;
        create_info.queueCreateInfoCount = 1;
        create_info.pEnabledFeatures = &device_features;

        const VkResult status = vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Vulkan logical device!");

        vkGetDeviceQueue(m_device, *m_queue_family_indices.graphics_family, 0, &m_graphics_queue);
        NB_CORE_ASSERT(m_graphics_queue != VK_NULL_HANDLE, "Unable to retrive Vulkan graphics queue handle!");
    }

    void VulkanContext::createSurface()
    {
        const VkResult status = glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Window surface!");
    }

}

VkApplicationInfo createApplicationInfo()
{
    auto& application = nebula::Application::get();
    auto [app_major, app_minor, app_patch] = application.getVersion();

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = application.getName().c_str();
    app_info.applicationVersion = VK_MAKE_VERSION(app_major, app_minor, app_patch);
    app_info.pEngineName = "Nebula";
    app_info.engineVersion = VK_MAKE_VERSION(NEBULA_MAJOR_VERSION, NEBULA_MINOR_VERSION, NEBULA_PATCH_VERSION);
    app_info.apiVersion = VK_MAKE_API_VERSION(0, VULKAN_MAJOR_VERSION, VULKAN_MINOR_VERSION, VULKAN_PATCH_VERSION);

    return app_info;
}

VkDeviceQueueCreateInfo createQueueInfo(uint32_t queue_family_index, float priority)
{
    static float queue_priority = 1.0f; //  Fight stupid pointer reference in struct
    queue_priority = priority;

    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family_index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    return queue_create_info;
}

int ratePhysicalDevice(const VkPhysicalDevice device)
{
    VkPhysicalDeviceFeatures device_features;
    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceFeatures(device, &device_features);
    vkGetPhysicalDeviceProperties(device, &device_properties);

    int score = 0;

    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000;

    score += static_cast<int>(device_properties.limits.maxImageDimension2D);

    return score;
}

QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    const auto queue_families = getQueueFamilies(device);
    for (int index = 0; index < queue_families.size(); ++index)
    {
        if (queue_families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphics_family = index;

        if (indices.checkMinimalSupport())
            break;
    }

    return indices;
}

std::vector<VkPhysicalDevice> getPhysicalDevices(const VkInstance instance)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    NB_CORE_ASSERT(device_count > 0, "Failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    return devices;
}

std::vector<VkQueueFamilyProperties> getQueueFamilies(const VkPhysicalDevice device)
{
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    return queue_families;
}

std::vector<VkExtensionProperties> getSupportedExtensions()
{
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> supported_extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, supported_extensions.data());

    return supported_extensions;
}

DeviceProperties getDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    int major = VK_API_VERSION_MAJOR(properties.apiVersion);
    int minor = VK_API_VERSION_MINOR(properties.apiVersion);
    int patch = VK_API_VERSION_PATCH(properties.apiVersion);

    const uint32_t vendor_id = properties.vendorID & 0xFFFF;
    std::string vendor_name;
    switch (vendor_id)
    {
        case 0x1002:    vendor_name = "AMD";        break;
        case 0x1010:    vendor_name = "ImgTec";     break;
        case 0x10DE:    vendor_name = "NVIDIA";     break;
        case 0x13B5:    vendor_name = "ARM";        break;
        case 0x5143:    vendor_name = "Qualcomm";   break;
        case 0x8086:    vendor_name = "INTEL";      break;
        default:        vendor_name = "Undefined";
    }

    const uint32_t driver_version = properties.driverVersion;
    const std::string driver_string = std::format("{}.{}", (driver_version >> 22) & 0x3ff, (driver_version >> 14) & 0x0ff);
    NB_CORE_ASSERT(vendor_name == "NVIDIA", "Vendor driver version not supported!");

    return {
        vendor_name,
        properties.deviceName,
        driver_string,
        std::format("{}.{}.{}", major, minor, patch)
    };
}

#ifdef NB_DEBUG_BUILD

bool checkValidationLayerSupport(const std::vector<const char*>& requested_layers)
{
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    return std::ranges::all_of(
        requested_layers,
        [&available_layers](const char* requested_layer)
        {
            return std::ranges::find_if(
                available_layers,
                [requested_layer](const VkLayerProperties& layer)
                {
                    return std::strcmp(requested_layer, layer.layerName) == 0;
                }
            ) != available_layers.end();
        }
    );
}

VkBool32 debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT flags,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data
)
{
    if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= 2)
        if (severity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
            return VK_FALSE;

    switch (severity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:     NB_CORE_FATAL(callback_data->pMessage); break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:   NB_CORE_WARN(callback_data->pMessage);  break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:      NB_CORE_INFO(callback_data->pMessage);  break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:   NB_CORE_TRACE(callback_data->pMessage); break;
        default:                                                NB_CORE_ASSERT(false, "Unknown severity level!");
    }

    return VK_FALSE;
}

VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= 2)
    {
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;
    }
    else
    {
        create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    }
    create_info.pfnUserCallback = debugCallback;
    create_info.pUserData = nullptr;

    return create_info;
}

VkResult createDebugUtilsMessengerEXT(const VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    NB_CORE_ASSERT(func, "DebugUtilsMessengerEXT not present!");
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void destroyDebugUtilsMessengerEXT(const VkInstance instance, const VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    NB_CORE_ASSERT(func, "DebugUtilsMessengerEXT not present!");
    func(instance, debugMessenger, pAllocator);
}

#endif

