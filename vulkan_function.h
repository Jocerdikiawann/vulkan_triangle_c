#ifndef VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

#define PANIC(...)                                                             \
  do {                                                                         \
    printf("@ %s (%d):", __FILE__, __LINE__);                                  \
    printf(__VA_ARGS__);                                                       \
  } while (0)

struct QueueFamilyIndices {
  uint32_t graphicFamily;
};

VkInstance createInstance();
void deleteVkInstance(VkInstance *vkInstance);
void deleteRequiredExtension(const char **requiredExtension);
void deleteVkExtensionProperties(VkExtensionProperties *properties);
void pickPhysicalDevices(VkInstance instance);
struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
void createLogicalDevice(VkPhysicalDevice physicalDevice, VkDevice device);
VkDevice createDevice();
VkQueue createGraphicsQueue();
void deleteDevice(VkDevice *device);

#endif // !VULKAN_FUNCTION_H
