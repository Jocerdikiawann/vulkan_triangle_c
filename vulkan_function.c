#include "vulkan_function.h"
#include <GLFW/glfw3.h>

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
  vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance);

  return instance;
}

void deleteVkInstance(VkInstance *vkInstance) {
  vkDestroyInstance(*vkInstance, NULL);
}
