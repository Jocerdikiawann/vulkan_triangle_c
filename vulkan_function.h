#ifndef VULKAN_FUNCTION_H
#define VULKAN_FUNCTION_H

#include "string.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan/vulkan.h"

#ifndef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif // !NDEBUG

#define PANIC(...)                                                             \
  do {                                                                         \
    printf("@ %s (%d):", __FILE__, __LINE__);                                  \
    printf(__VA_ARGS__);                                                       \
    printf("\n");                                                              \
    exit(1);                                                                   \
  } while (0)

typedef struct QueueFamilyIndices {
  uint32_t graphicFamily;
  uint32_t presentFamily;
} QueueFamilyIndices;

bool isComplete(QueueFamilyIndices *p);

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
int rateDeviceSuitability(VkPhysicalDevice device);

VkInstance createInstance();

VkPhysicalDevice pickPhysicalDevices(VkInstance instance);

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocators,
    VkDebugUtilsMessengerEXT *pDebugMessenger);

VkDebugUtilsMessengerCreateInfoEXT createInfoMessenger();

VkDebugUtilsMessengerEXT createDebugMessenger(VkInstance);

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallBack(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface);

VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice,
                             VkSurfaceKHR surface);

VkQueue createGraphicsQueue(VkDevice device, QueueFamilyIndices indices);

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window);

VkQueue presentQueue(VkDevice device, VkPhysicalDevice physicalDevice,
                     VkSurfaceKHR surface);

bool checkValidationLayerSupport();

void deleteDevice(VkDevice *device);

void destroySurface(VkInstance instance, VkSurfaceKHR surface);

void deleteVkInstance(VkInstance *vkInstance);

void deleteRequiredExtension(const char **requiredExtension);

void deleteVkExtensionProperties(VkExtensionProperties *properties);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   const VkAllocationCallbacks *pAllocators,
                                   VkDebugUtilsMessengerEXT pDebugMessenger);

void destroyDevice(VkDevice device);

#endif // !VULKAN_FUNCTION_H
