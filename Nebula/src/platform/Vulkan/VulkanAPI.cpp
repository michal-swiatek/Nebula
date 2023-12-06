//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/Vulkan/VulkanAPI.h"

#include <set>
#include <map>

#include <GLFW/glfw3.h>

#include "core/Assert.h"
#include "core/Logging.h"
#include "core/Application.h"

#include "platform/Vulkan/VulkanSwapchain.h"
#include "platform/EngineConfiguration.h"
#include "platform/Vulkan/VulkanConfiguration.h"

//  Device helpers
int ratePhysicalDevice(VkPhysicalDevice device);
bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& required_extensions);

//  Create helpers
VkApplicationInfo createApplicationInfo();
VkDeviceQueueCreateInfo createQueueInfo(uint32_t queue_family_index, float priority = 1.0);

//  Listing helpers
std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance);
std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device);
std::vector<VkExtensionProperties> getDeviceExtensions(VkPhysicalDevice device);
std::vector<VkExtensionProperties> getSupportedExtensions();

#ifdef NB_DEBUG_BUILD
bool checkValidationLayerSupport(const std::vector<const char*>& requested_layers);
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void* user_data);

VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo();
VkResult createDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT*, const VkAllocationCallbacks*, VkDebugUtilsMessengerEXT*);
void destroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks*);
#endif

namespace nebula::rendering {

    VkInstance VulkanAPI::s_instance = VK_NULL_HANDLE;
    VkDevice VulkanAPI::s_device = VK_NULL_HANDLE;
    VkPhysicalDevice VulkanAPI::s_physical_device = VK_NULL_HANDLE;
    VmaAllocator VulkanAPI::s_vma_allocator = VK_NULL_HANDLE;
    QueuesInfo VulkanAPI::s_queues_info;

    VkInstance VulkanAPI::getInstance() { return s_instance; }
    VkDevice VulkanAPI::getDevice() { return s_device; }
    VkPhysicalDevice VulkanAPI::getPhysicalDevice() { return s_physical_device; }
    VmaAllocator VulkanAPI::getVmaAllocator() { return s_vma_allocator; }

    Scope<VulkanAPI> VulkanAPI::create(GLFWwindow* window)
    {
        return createScope<VulkanAPI>(window);
    }

    VulkanAPI::VulkanAPI(GLFWwindow* window)
    {
        m_device_required_extensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        createVulkanInstance();
        createSurface(window);
        createPhysicalDevice();
        createLogicalDevice();
        createVmaAllocator();

        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_HIGH)
        {
            auto supported_extensions = getSupportedExtensions();
            NB_CORE_INFO("Vulkan extensions:");
            for (const auto& [extension_name, spec_version] : supported_extensions)
                NB_CORE_INFO("  {0}", extension_name);
        }
    }

    VulkanAPI::~VulkanAPI()
    {
        NB_CORE_ASSERT(s_instance, "No Vulkan instance!");
        NB_CORE_ASSERT(s_device, "No Vulkan device!");
        NB_CORE_ASSERT(s_physical_device, "No Vulkan physical device!");
        NB_CORE_ASSERT(s_vma_allocator, "No VMA Allocator!");

        vmaDestroyAllocator(s_vma_allocator);
        vkDestroyDevice(s_device, nullptr);
        vkDestroySurfaceKHR(s_instance, m_surface, nullptr);

        #ifdef NB_DEBUG_BUILD
        destroyDebugUtilsMessengerEXT(s_instance, m_debug_messenger, nullptr);
        #endif

        vkDestroyInstance(s_instance, nullptr);

        s_instance = VK_NULL_HANDLE;
        s_device = VK_NULL_HANDLE;
        s_physical_device = VK_NULL_HANDLE;
        s_queues_info = {};
    }

    void VulkanAPI::createVulkanInstance()
    {
        NB_CORE_ASSERT(!s_instance, "Can have only one Vulkan instance!");

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

        const VkResult status = vkCreateInstance(&create_info, nullptr, &s_instance);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to initialize Vulkan!");

        #ifdef NB_DEBUG_BUILD
        const VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = createDebugUtilsMessengerInfo();
        NB_CORE_ASSERT(
            createDebugUtilsMessengerEXT(s_instance, &debug_messenger_info, nullptr, &m_debug_messenger) == VK_SUCCESS,
            "Unable to set Vulkan debug callback!"
        );
        if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_LOW)
            NB_CORE_INFO("Using Vulkan validation layers");
        #endif
    }

    void VulkanAPI::createPhysicalDevice()
    {
        NB_CORE_ASSERT(!s_physical_device, "Can have only one Vulkan physical device!");

        const auto devices = getPhysicalDevices(s_instance);
        std::multimap<int, VkPhysicalDevice> candidates;

        for (const auto& device : devices)
        {
            if (isDeviceSuitable(device))
                candidates.insert(std::make_pair(ratePhysicalDevice(device), device));
        }

        NB_CORE_ASSERT(candidates.rbegin()->first > 0, "Failed to find a suitable GPU!")
        s_physical_device = candidates.rbegin()->second;

        m_queue_family_indices = findQueueFamilies(s_physical_device);
        s_queues_info.graphics_family_index = *m_queue_family_indices.graphics_family;
        s_queues_info.presentation_family_index = *m_queue_family_indices.presentation_family;
    }

    void VulkanAPI::createLogicalDevice()
    {
        NB_CORE_ASSERT(!s_device, "Can have only one Vulkan device!");

        const auto [graphics_family, presentation_family] = m_queue_family_indices;    //  For shorter typing

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        const std::set unique_queue_indices = {*graphics_family, *presentation_family};

        queue_create_infos.reserve(unique_queue_indices.size());
        for (const auto queue_index : unique_queue_indices)
            queue_create_infos.push_back(createQueueInfo(queue_index));

        VkPhysicalDeviceFeatures device_features{};

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        create_info.pEnabledFeatures = &device_features;
        create_info.enabledExtensionCount = static_cast<uint32_t>(m_device_required_extensions.size());
        create_info.ppEnabledExtensionNames = m_device_required_extensions.data();

        const VkResult status = vkCreateDevice(s_physical_device, &create_info, nullptr, &s_device);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Vulkan logical device!");

        vkGetDeviceQueue(s_device, *graphics_family, 0, &s_queues_info.graphics_queue);
        NB_CORE_ASSERT(s_queues_info.graphics_queue != VK_NULL_HANDLE, "Unable to retrive Vulkan graphics queue handle!");

        vkGetDeviceQueue(s_device, *presentation_family, 0, &s_queues_info.presentation_queue);
        NB_CORE_ASSERT(s_queues_info.presentation_queue != VK_NULL_HANDLE, "Unable to retrive Vulkan presentation queue handle!");
    }

    void VulkanAPI::createVmaAllocator()
    {
        VmaAllocatorCreateInfo create_info{};

        create_info.vulkanApiVersion = VK_MAKE_API_VERSION(0, VULKAN_MAJOR_VERSION, VULKAN_MINOR_VERSION, VULKAN_PATCH_VERSION);
        create_info.instance = getInstance();
        create_info.physicalDevice = getPhysicalDevice();
        create_info.device = getDevice();

        vmaCreateAllocator(&create_info, &s_vma_allocator);
    }

    void VulkanAPI::createSurface(GLFWwindow* window)
    {
        const VkResult status = glfwCreateWindowSurface(s_instance, window, nullptr, &m_surface);
        NB_CORE_ASSERT(status == VK_SUCCESS, "Failed to create Window surface!");
    }

    bool VulkanAPI::isDeviceSuitable(VkPhysicalDevice device) const
    {
        if (checkDeviceExtensionSupport(device, m_device_required_extensions))
        {
            const auto queue_indices = findQueueFamilies(device);
            const auto swapchain_details = querySwapchainSupport(device, m_surface);

            return queue_indices.checkMinimalSupport() && !swapchain_details.formats.empty() && !swapchain_details.present_modes.empty();
        }

        return false;
    }

    VulkanAPI::QueueFamilyIndices VulkanAPI::findQueueFamilies(VkPhysicalDevice device) const
    {
        QueueFamilyIndices indices;
        VkBool32 present_support = false;

        const auto queue_families = getQueueFamilies(device);
        for (int index = 0; index < queue_families.size(); ++index)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(device, index, m_surface, &present_support);

            if (queue_families[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.graphics_family = index;

            if (present_support)
                indices.presentation_family = index;

            if (indices.checkMinimalSupport())
                break;
        }

        return indices;
    }

    QueuesInfo VulkanAPI::getQueuesInfo()
    {
        NB_CORE_ASSERT(s_instance, "Vulkan instance is needed to access Queues!");
        return s_queues_info;
    }

    PhysicalDeviceInfo VulkanAPI::getPhysicalDeviceInfo()
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(s_physical_device, &properties);

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

}

/////////////////////////////////////////////////////////////////////////////////
////  Device helpers  ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////
////  Create helpers  ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////
////  Listing helpers  //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance)
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    NB_CORE_ASSERT(device_count > 0, "Failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    return devices;
}

std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device)
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

/////////////////////////////////////////////////////////////////////////////////
////  Debug functions  //////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

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
    if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_MEDIUM)
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
    if constexpr (NEBULA_INITIALIZATION_VERBOSITY >= NEBULA_INITIALIZATION_VERBOSITY_MEDIUM)
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

VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
    NB_CORE_ASSERT(func, "DebugUtilsMessengerEXT not present!");
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
    NB_CORE_ASSERT(func, "DebugUtilsMessengerEXT not present!");
    func(instance, debugMessenger, pAllocator);
}

#endif
