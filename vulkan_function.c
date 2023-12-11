#include "vulkan_function.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

VkInstance createInstance() {
  VkApplicationInfo appInfo = {
      VK_STRUCTURE_TYPE_APPLICATION_INFO,
      VK_NULL_HANDLE,
      "HELLO_TRIANGLE",
      VK_MAKE_VERSION(1, 0, 0),
      "No Engine",
      VK_MAKE_VERSION(1, 0, 0),
      VK_API_VERSION_1_0,
  };

  uint32_t glfwExtensionsCount = 0;

  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

  VkInstanceCreateInfo createInfo = {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      VK_NULL_HANDLE,
      0,
      &appInfo,
      0,
      VK_NULL_HANDLE,
      glfwExtensionsCount,
      glfwExtensions,
  };

  VkInstance instance;

  const char **requiredExtension =
      malloc((glfwExtensionsCount + 1) * sizeof(char *));

  for (uint32_t i = 0; i < glfwExtensionsCount; i++) {
    requiredExtension[i] = glfwExtensions[i];
  }

  requiredExtension[glfwExtensionsCount] =
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;

  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  createInfo.enabledExtensionCount = glfwExtensionsCount + 1;
  createInfo.ppEnabledLayerNames = requiredExtension;

  if (vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance) != VK_SUCCESS) {
    fprintf(stderr, "failed to create instance\n");
    exit(EXIT_FAILURE);
  }

  free(requiredExtension);

  return instance;
}

void deleteVkInstance(VkInstance *vkInstance) {
  vkDestroyInstance(*vkInstance, NULL);
}
