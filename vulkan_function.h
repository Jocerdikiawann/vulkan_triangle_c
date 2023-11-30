#ifndef VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

VkInstance createInstance();
void deleteVkInstance(VkInstance *vkInstance);

#endif // !VULKAN_FUNCTION_H
