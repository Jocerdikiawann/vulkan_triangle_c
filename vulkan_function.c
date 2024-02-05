#include "vulkan_function.h"

const char *validationLayers[] = {"VK_LAYER_KHRONOS_validation"};
const uint32_t validationLayerCount = 1;
float queuePriority = 1.0f;

VkInstance createInstance() {

  if (enableValidationLayers && !checkValidationLayerSupport()) {
    PANIC("Validation layer requested but not available");
  }

  VkInstance instance;

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
  const char **glfwExtensions;

  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);

  const char *glfwExtensionsWithDebug[glfwExtensionsCount + 1];

  for (uint32_t i = 0; i < glfwExtensionsCount; i++) {
    glfwExtensionsWithDebug[i] = glfwExtensions[i];
  }

  VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
  };

  VkDebugUtilsMessengerCreateInfoEXT debugMessenger = createInfoMessenger();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.enabledExtensionCount = glfwExtensionsCount + 1;
    createInfo.ppEnabledExtensionNames = glfwExtensionsWithDebug;
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugMessenger;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.enabledExtensionCount = glfwExtensionsCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.pNext = NULL;
  }

  VkResult result = vkCreateInstance(&createInfo, VK_NULL_HANDLE, &instance);

  if (result != VK_SUCCESS) {
    PANIC("Failed to create instance");
  }

  uint32_t extensionCount = 0;

  vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extensionCount,
                                         VK_NULL_HANDLE);

  VkExtensionProperties *extensions = (VkExtensionProperties *)malloc(
      extensionCount * sizeof(VkExtensionProperties));

  vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extensionCount,
                                         extensions);

  printf("Available extension:\n");
  for (uint32_t i = 0; i < extensionCount; i++) {
    printf("\t%s\n", extensions[i].extensionName);
  }

  free(extensions);
  extensions = NULL;

  return instance;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *pUserData) {
  printf("validation layer:%s\n", pCallbackData->pMessage);
  return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocators,
    VkDebugUtilsMessengerEXT *pDebugMessenger) {
  PFN_vkCreateDebugUtilsMessengerEXT func =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != NULL) {
    return func(instance, pCreateInfo, pAllocators, pDebugMessenger);
  }

  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

VkDebugUtilsMessengerCreateInfoEXT createInfoMessenger() {

  VkDebugUtilsMessengerCreateInfoEXT createInfo = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
      .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
      .pfnUserCallback = debugCallBack,
      .pUserData = VK_NULL_HANDLE,
  };

  return createInfo;
}

VkDebugUtilsMessengerEXT createDebugMessenger(VkInstance instance) {
  if (!enableValidationLayers) {
    return NULL;
  }

  VkDebugUtilsMessengerEXT debugMessenger;

  VkDebugUtilsMessengerCreateInfoEXT createInfo = createInfoMessenger();

  if (CreateDebugUtilsMessengerEXT(instance, &createInfo, VK_NULL_HANDLE,
                                   &debugMessenger) != VK_SUCCESS) {
    PANIC("failed to set up debug messenger");
  }
  return debugMessenger;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface) {
  QueueFamilyIndices indices;
  uint32_t queueFamilyCount = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           VK_NULL_HANDLE);

  VkQueueFamilyProperties queueFamilies[queueFamilyCount];

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies);

  for (uint32_t i = 0; i < queueFamilyCount; i++) {

    if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicFamily = i;
    } else {
      indices.graphicFamily = 0;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (isComplete(&indices)) {
      break;
    }
  }

  return indices;
}

bool checkValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, VK_NULL_HANDLE);

  VkLayerProperties *availableLayers =
      (VkLayerProperties *)malloc(layerCount * sizeof(VkLayerProperties));

  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

  for (uint32_t i = 0; i < validationLayerCount; i++) {
    bool layerFound = false;
    for (uint32_t j = 0; j < layerCount; j++) {
      if (strcmp(availableLayers[j].layerName, validationLayers[i]) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      free(availableLayers);
      availableLayers = NULL;
      return false;
    }
  }

  free(availableLayers);
  availableLayers = NULL;

  return true;
}

VkQueue createGrapichsQueue(VkDevice device, QueueFamilyIndices indices) {
  VkQueue graphicsQueue;
  vkGetDeviceQueue(device, indices.graphicFamily, 0, &graphicsQueue);
  return graphicsQueue;
}

VkDeviceQueueCreateInfo *
getDeviceQueueCreateInfo(size_t uniqueQueueFamiliesSize,
                         uint32_t *uniqueQueueFamilies) {
  VkDeviceQueueCreateInfo *queueCreateInfos =
      (VkDeviceQueueCreateInfo *)malloc(sizeof(VkDeviceQueueCreateInfo) * 2);

  for (size_t i = 0; i < uniqueQueueFamiliesSize; i++) {
    VkDeviceQueueCreateInfo queueCreateInfo;
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = uniqueQueueFamilies[i];
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos[i] = queueCreateInfo;
  }
  return queueCreateInfos;
}

VkQueue createPresentQueue(QueueFamilyIndices indices, VkDevice device,
                           VkPhysicalDevice physicalDevice,
                           VkSurfaceKHR surface) {
  VkQueue presentQueue;
  vkGetDeviceQueue(device, indices.graphicFamily, 0, &presentQueue);

  return presentQueue;
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window) {
  VkSurfaceKHR surface;
  if (glfwCreateWindowSurface(instance, window, VK_NULL_HANDLE, &surface) !=
      VK_SUCCESS) {
    PANIC("failed to create window surface");
  }
  return surface;
}

VkDevice createLogicalDevice(QueueFamilyIndices indices,
                             VkPhysicalDevice physicalDevice,
                             VkSurfaceKHR surface) {

  size_t uniqueQueueFamiliesSize = 2;
  uint32_t uniqueQueueFamilies[uniqueQueueFamiliesSize];
  uniqueQueueFamilies[0] = indices.graphicFamily;
  uniqueQueueFamilies[1] = indices.presentFamily;

  VkDeviceQueueCreateInfo *queueCreateInfos =
      getDeviceQueueCreateInfo(uniqueQueueFamiliesSize, uniqueQueueFamilies);

  VkPhysicalDeviceFeatures deviceFeatures = {};
  VkDeviceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .pQueueCreateInfos = queueCreateInfos,
      .queueCreateInfoCount = uniqueQueueFamiliesSize,
      .pEnabledFeatures = &deviceFeatures,
      .enabledExtensionCount = 0,
  };

  if (enableValidationLayers) {
    createInfo.enabledLayerCount = validationLayerCount;
    createInfo.ppEnabledLayerNames = validationLayers;
  } else {
    createInfo.enabledLayerCount = 0;
  }

  VkDevice device;

  if (vkCreateDevice(physicalDevice, &createInfo, VK_NULL_HANDLE, &device) !=
      VK_SUCCESS) {
    PANIC("failed to create logical device!");
  }

  free(queueCreateInfos);
  queueCreateInfos = NULL;

  return device;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  QueueFamilyIndices indices = findQueueFamilies(device, surface);
  return isComplete(&indices);
}

VkPhysicalDevice pickPhysicalDevices(VkInstance instance) {
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  uint32_t deviceCount = 0;

  vkEnumeratePhysicalDevices(instance, &deviceCount, VK_NULL_HANDLE);

  if (deviceCount == 0) {
    PANIC("Failed to find GPUs with vulkan support");
  }

  VkPhysicalDevice *devices =
      (VkPhysicalDevice *)malloc(deviceCount * sizeof(VkPhysicalDevice));

  vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

  if (physicalDevice == VK_NULL_HANDLE) {
    PANIC("failed to find a suitable GPU");
  }

  VkPhysicalDevice device;
  uint32_t deviceScore = 0;

  for (uint32_t i = 0; deviceCount > i; i++) {
    uint32_t score = rateDeviceSuitability(devices[i]);
    if (score > deviceScore) {
      deviceScore = score;
      device = devices[i];
    }
  }

  if (device == VK_NULL_HANDLE) {
    PANIC("failed to find a suitable GPU");
  }

  free(devices);
  devices = NULL;

  return device;
}

int rateDeviceSuitability(VkPhysicalDevice device) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeature;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeature);

  int score = 0;
  if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  score += deviceProperties.limits.maxImageDimension2D;

  if (!deviceFeature.geometryShader) {
    return 0;
  }

  return score;
}

bool isComplete(QueueFamilyIndices *p) {
  return p->graphicFamily > 0 && p->presentFamily > 0;
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

void destroySurface(VkInstance instance, VkSurfaceKHR surface) {
  vkDestroySurfaceKHR(instance, surface, VK_NULL_HANDLE);
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   const VkAllocationCallbacks *pAllocators,
                                   VkDebugUtilsMessengerEXT pDebugMessenger) {
  PFN_vkDestroyDebugUtilsMessengerEXT func =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          instance, "vkDestroyDebugUtilsMessengerEXT");

  if (func != NULL) {
    func(instance, pDebugMessenger, pAllocators);
  }
}

void destroyDevice(VkDevice device) {
  vkDestroyDevice(device, VK_NULL_HANDLE);
};
