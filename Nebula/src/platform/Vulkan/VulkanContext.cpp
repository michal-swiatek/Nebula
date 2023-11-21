//
// Created by michal-swiatek on 15.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanContext.h"

#include <set>
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

//  Create helpers
VkApplicationInfo createApplicationInfo();
VkDeviceQueueCreateInfo createQueueInfo(uint32_t queue_family_index, float priority = 1.0);

//  Listing helpers
std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance);
std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);
std::vector<VkExtensionProperties> getDeviceExtensions(VkPhysicalDevice device);
std::vector<VkExtensionProperties> getSupportedExtensions();

//  Device helpers
int ratePhysicalDevice(VkPhysicalDevice device);
bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& required_extensions);
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*>& required_extensions);
DeviceProperties getDeviceProperties(VkPhysicalDevice device);

#ifdef NB_DEBUG_BUILD
bool checkValidationLayerSupport(const std::vector<const char*>& requested_layers);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void* user_data);

VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo();
VkResult createDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);
void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);
#endif

namespace nebula::rendering {

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    VulkanContext::VulkanContext(GLFWwindow* window_handle) : m_window(window_handle)
    {
        NB_CORE_ASSERT(m_window, "Window handle in null!");

        //  Required extensions
        m_device_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        createVulkanInstance();
        createSurface();
        createPhysicalDevice();
        createLogicalDevice();
        createSwapchain();

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

        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
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

    void VulkanContext::createSurface()
    {
        const VkResult status = glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Window surface!");
    }

    void VulkanContext::createPhysicalDevice()
    {
        const auto devices = getPhysicalDevices(m_instance);
        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices)
        {
            if (isDeviceSuitable(device, m_surface, m_device_extensions))
                candidates.insert(std::make_pair(ratePhysicalDevice(device), device));
        }

        NB_CORE_ASSERT(candidates.rbegin()->first > 0, "Failed to find a suitable GPU!")
        m_physical_device = candidates.rbegin()->second;
        m_queue_family_indices = findQueueFamilies(m_physical_device, m_surface);
        m_swapchain_details = querySwapchainSupport(m_physical_device, m_surface);
    }

    void VulkanContext::createLogicalDevice()
    {
        const auto [graphics_family, presentation_family] = m_queue_family_indices;    //  For shorter typing

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set unique_queue_indices = {*graphics_family, *presentation_family};

        queue_create_infos.reserve(unique_queue_indices.size());
        for (const auto queue_index : unique_queue_indices)
            queue_create_infos.push_back(createQueueInfo(queue_index));

        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        create_info.pEnabledFeatures = &device_features;
        create_info.enabledExtensionCount = static_cast<uint32_t>(m_device_extensions.size());
        create_info.ppEnabledExtensionNames = m_device_extensions.data();

        const VkResult status = vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Vulkan logical device!");

        vkGetDeviceQueue(m_device, *graphics_family, 0, &m_graphics_queue);
        NB_CORE_ASSERT(m_graphics_queue != VK_NULL_HANDLE, "Unable to retrive Vulkan graphics queue handle!");

        vkGetDeviceQueue(m_device, *presentation_family, 0, &m_presentation_queue);
        NB_CORE_ASSERT(m_presentation_queue != VK_NULL_HANDLE, "Unable to retrive Vulkan presentation queue handle!");
    }

    void VulkanContext::createSwapchain()
    {
        m_surface_format = chooseSwapSurfaceFormat();
        m_present_mode = chooseSwapPresentMode();
        m_extent = chooseSwapExtent();

        uint32_t image_count = m_swapchain_details.capabilities.minImageCount + 1;
        if (m_swapchain_details.capabilities.maxImageCount > 0 && image_count > m_swapchain_details.capabilities.maxImageCount)
            image_count = m_swapchain_details.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        create_info.surface = m_surface;
        create_info.minImageCount = image_count;
        create_info.imageFormat = m_surface_format.format;
        create_info.imageColorSpace = m_surface_format.colorSpace;
        create_info.imageExtent = m_extent;
        create_info.imageArrayLayers = 1;
        create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        create_info.preTransform = m_swapchain_details.capabilities.currentTransform;
        create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        create_info.presentMode = m_present_mode;
        create_info.clipped = VK_TRUE;
        create_info.oldSwapchain = m_swapchain;

        uint32_t queue_family_indices[] = {*m_queue_family_indices.graphics_family, *m_queue_family_indices.presentation_family};
        if (m_queue_family_indices.graphics_family != m_queue_family_indices.presentation_family)
        {
            create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            create_info.queueFamilyIndexCount = 2;
            create_info.pQueueFamilyIndices = queue_family_indices;
        }
        else
        {
            create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            create_info.queueFamilyIndexCount = 0;
            create_info.pQueueFamilyIndices = nullptr;
        }

        const VkResult status = vkCreateSwapchainKHR(m_device, &create_info, nullptr, &m_swapchain);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create swapchain!");
    }

    VkSurfaceFormatKHR VulkanContext::chooseSwapSurfaceFormat() const
    {
        for (const auto& format : m_swapchain_details.formats)
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;
        return m_swapchain_details.formats[0];
    }

    VkPresentModeKHR VulkanContext::chooseSwapPresentMode() const
    {
        for (const auto& present_mode : m_swapchain_details.present_modes)
            if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return present_mode;
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanContext::chooseSwapExtent() const
    {
        const auto capabilities = m_swapchain_details.capabilities;
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;

        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);

        VkExtent2D extent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return extent;
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;
        VkBool32 present_support = false;

        const auto queue_families = getQueueFamilies(device);
        for (int index = 0; index < queue_families.size(); ++index)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &present_support);

            if (queue_families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphics_family = index;

            if (present_support)
                indices.presentation_family = index;

            if (indices.checkMinimalSupport())
                break;
        }

        return indices;
    }

    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapchainSupportDetails details;
        uint32_t format_count;
        uint32_t present_mode_count;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

        if (format_count > 0)
        {
            details.formats.resize(format_count);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
        }

        if (present_mode_count > 0)
        {
            details.present_modes.resize(present_mode_count);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
        }

        return details;
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

VkDeviceQueueCreateInfo createQueueInfo(const uint32_t queue_family_index, float priority)
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

int ratePhysicalDevice(VkPhysicalDevice device)
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

bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& required_extensions)
{
    auto device_extensions = getDeviceExtensions(device);

    for (const auto& extension : required_extensions)
    {
        auto it = std::ranges::find_if(device_extensions, [&extension](const auto& properties){ return std::strcmp(properties.extensionName, extension) == 0; });
        if (it == device_extensions.end())
            return false;
    }

    return true;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*>& required_extensions)
{
    if (checkDeviceExtensionSupport(device, required_extensions))
    {
        const auto queue_indices = nebula::rendering::findQueueFamilies(device, surface);
        const auto swapchain_details = nebula::rendering::querySwapchainSupport(device, surface);

        return queue_indices.checkMinimalSupport() && !swapchain_details.formats.empty() && !swapchain_details.present_modes.empty();
    }

    return false;
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

std::vector<VkExtensionProperties> getDeviceExtensions(VkPhysicalDevice device)
{
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> device_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, device_extensions.data());

    return device_extensions;
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

