#ifndef VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#include "vulkan/vulkan.h"

VkInstance createInstance();
void deleteVkInstance(VkInstance *vkInstance);

#endif // !VULKAN_FUNCTION_H
