#include "vulkan_function.h"
#include "window.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main() {
  glfwInit();
  GLFWwindow *window = createWindow(WIDTH, HEIGHT, "Triangle");
  VkInstance vkInstance = createInstance();
  VkSurfaceKHR surface = createSurface(vkInstance, window);
  VkPhysicalDevice physicalDevice = pickPhysicalDevices(vkInstance);
  VkDevice device = createLogicalDevice(physicalDevice, surface);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  deleteWindow(window);
  deleteVkInstance(&vkInstance);

  return 0;
  // mat4 matrix;
  // glm_mat4_zero(matrix);

  // vec4 result;
  // glm_mat4_mulv(matrix, vec, result);
}
