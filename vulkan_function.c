#include "vulkan_function.h"
#include <stdbool.h>
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
  if (requiredExtension == NULL) {
    exit(EXIT_FAILURE);
  }

  for (uint32_t i = 0; i < glfwExtensionsCount; i++) {
    requiredExtension[i] = glfwExtensions[i];
  }

  requiredExtension[glfwExtensionsCount] =
      VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;

  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
  createInfo.enabledExtensionCount = glfwExtensionsCount + 1;
  createInfo.ppEnabledLayerNames = requiredExtension;

  if (vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance) != VK_SUCCESS) {
    PANIC("ERROR:", stderr);
    deleteRequiredExtension(requiredExtension);
    exit(EXIT_FAILURE);
  }

  uint32_t extensionCount = 0;

  VkExtensionProperties *extensions = (VkExtensionProperties *)malloc(
      extensionCount * sizeof(VkExtensionProperties));

  if (extensions == NULL) {
    exit(EXIT_FAILURE);
  }

  vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extensionCount,
                                         extensions);

  for (uint32_t i = 0; i < extensionCount; i++) {
    printf("\t%s\n", extensions[i].extensionName);
  }

  return instance;
}

struct QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
  struct QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           VK_NULL_HANDLE);
  VkQueueFamilyProperties *queueFamilies = (VkQueueFamilyProperties *)malloc(
      queueFamilyCount * sizeof(VkQueueFamilyProperties));

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {
    if ((queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) {
      indices.graphicFamily = i;
    }
  }

  free(queueFamilies);
  queueFamilies = NULL;
  return indices;
}

VkDevice createDevice() { return NULL; }
VkQueue createGrapichsQueue() { return NULL; }

void createLogicalDevice(VkPhysicalDevice physicalDevice, VkDevice device) {
  VkQueue graphicQueue;
  struct QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
  VkDeviceQueueCreateInfo queueCreateInfo = {};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = indices.graphicFamily;
  queueCreateInfo.queueCount = 1;

  float queuePriority = 1.0f;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeature = {};

  VkDeviceCreateInfo createInfo = {};

  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pEnabledFeatures = &deviceFeature;
  createInfo.enabledExtensionCount = 0;
  createInfo.enabledLayerCount = 0;

  if (vkCreateDevice(physicalDevice, &createInfo, VK_NULL_HANDLE, &device)) {
    PANIC("Failed to create logical device");
  }
  vkGetDeviceQueue(device, indices.graphicFamily, 0, &graphicQueue);
}

void pickPhysicalDevices(VkInstance instance) {
  uint32_t deviceCount = 0;

  VkPhysicalDevice *devices =
      (VkPhysicalDevice *)malloc(deviceCount * sizeof(VkPhysicalDevice));

  vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

  if (deviceCount == 0) {
    PANIC("Failed to find GPUs with vulkan support");
  }

  if (devices == VK_NULL_HANDLE) {
    PANIC("Failed to find a suitable GPU");
  }

  free(devices);
  devices = NULL;
}

void deleteRequiredExtension(const char **requiredExtension) {
  free(requiredExtension);
  requiredExtension = NULL;
}

void deleteVkExtensionProperties(VkExtensionProperties *properties) {
  free(properties);
  properties = NULL;
}

void deleteVkInstance(VkInstance *vkInstance) {
  vkDestroyInstance(*vkInstance, NULL);
}

void deleteDevice(VkDevice *device) {
  vkDestroyDevice(*device, VK_NULL_HANDLE);
}
