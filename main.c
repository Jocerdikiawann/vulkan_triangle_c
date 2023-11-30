#include "vulkan_function.h"
#include <GLFW/glfw3.h>
#include <stdint.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "window.h"

#include <vulkan/vulkan.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

int main() {
  glfwInit();
  GLFWwindow *window = createWindow(WIDTH, HEIGHT, "Triangle");
  VkInstance vkInstance = createInstance();

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
