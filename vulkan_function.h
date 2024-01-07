#ifndef VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

#define PANIC(...)                                                             \
  do {                                                                         \
    printf("@ %s (%d):", __FILE__, __LINE__);                                  \
    printf(__VA_ARGS__);                                                       \
  } while (0)

VkInstance createInstance();
void deleteVkInstance(VkInstance *vkInstance);
void deleteRequiredExtension(const char **requiredExtension);
void deleteVkExtensionProperties(VkExtensionProperties *properties);

#endif // !VULKAN_FUNCTION_H
