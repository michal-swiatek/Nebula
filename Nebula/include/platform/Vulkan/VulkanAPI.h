//
// Created by michal-swiatek on 26.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef VULKANAPI_H
#define VULKANAPI_H

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include <string>
#include <optional>

#include "core/Types.h"

struct GLFWwindow;

namespace nebula::rendering {

    class VulkanContext;

    struct PhysicalDeviceInfo
    {
        std::string vendor{};
        std::string renderer{};
        std::string driver_version{};
        std::string api_version{};
    };

    struct QueuesInfo
    {
        VkQueue graphics_queue;
        VkQueue presentation_queue;

        uint32_t graphics_family_index;
        uint32_t presentation_family_index;
    };

    struct VkApiAllocatedImage
    {
        VkImage image = VK_NULL_HANDLE;
        VmaAllocation allocation{};
    };

    struct VkApiAllocatedBuffer
    {
        VkBuffer buffer = VK_NULL_HANDLE;
        VmaAllocation allocation{};
    };

    class VulkanAPI
    {
    public:
        explicit VulkanAPI(GLFWwindow* window);
        ~VulkanAPI();

        static VkInstance getInstance();
        static VkDevice getDevice();
        static VkPhysicalDevice getPhysicalDevice();
        static VmaAllocator getVmaAllocator();

        static QueuesInfo getQueuesInfo();
        static PhysicalDeviceInfo getPhysicalDeviceInfo();

    private:
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> graphics_family{};
            std::optional<uint32_t> presentation_family{};

            [[nodiscard]] bool checkMinimalSupport() const
            {
                return graphics_family && presentation_family;
            }
        };

        std::vector<const char*> m_device_required_extensions{};

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        QueueFamilyIndices m_queue_family_indices{};

        void createVulkanInstance();
        void createPhysicalDevice();
        void createLogicalDevice();
        void createVmaAllocator();
        void createSurface(GLFWwindow* window);

        bool isDeviceSuitable(VkPhysicalDevice device) const;
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

        static VkInstance s_instance;
        static VkDevice s_device;
        static VkPhysicalDevice s_physical_device;
        static VmaAllocator s_vma_allocator;
        static QueuesInfo s_queues_info;

        friend class nebula::rendering::VulkanContext;

        //  Called by VulkanContext
        static Scope<VulkanAPI> create(GLFWwindow* window);
        [[nodiscard]] VkSurfaceKHR getSurface() const { return m_surface; }

        #ifdef NB_DEBUG_BUILD
        VkDebugUtilsMessengerEXT m_debug_messenger{};
        #endif
    };

}

#endif //VULKANAPI_H
