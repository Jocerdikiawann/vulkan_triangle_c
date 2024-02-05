#include "vulkan_function.h"
#include "window.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main() {
  glfwInit();
  GLFWwindow *window = createWindow(WIDTH, HEIGHT, "Triangle");
  VkInstance vkInstance = createInstance();
  VkDebugUtilsMessengerEXT debugMessenger = createDebugMessenger(vkInstance);
  VkSurfaceKHR surface = createSurface(vkInstance, window);
  VkPhysicalDevice physicalDevice = pickPhysicalDevices(vkInstance);
  QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
  VkDevice device = createLogicalDevice(indices, physicalDevice, surface);
  VkQueue graphicQueue = createGraphicsQueue(device, indices);
  VkQueue presentQueue =
      createPresentQueue(indices, device, physicalDevice, surface);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  deleteWindow(window);
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(vkInstance, VK_NULL_HANDLE, debugMessenger);
  }
  destroySurface(vkInstance, surface);
  deleteVkInstance(&vkInstance);
  destroyDevice(device);

  return 0;
  // mat4 matrix;
  // glm_mat4_zero(matrix);

  // vec4 result;
  // glm_mat4_mulv(matrix, vec, result);
}
