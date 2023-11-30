#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "window.h"

#include <vulkan/vulkan.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

VkInstance createInstance();
void initVulkan();
void loop(GLFWwindow *window);

int main() {
  glfwInit();
  GLFWwindow *window = createWindow(WIDTH, HEIGHT, "Triangle");
  initVulkan();

  loop(window);
  deleteWindow(window);

  return 0;
  // mat4 matrix;
  // glm_mat4_zero(matrix);

  // vec4 result;
  // glm_mat4_mulv(matrix, vec, result);
}

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
  VkInstanceCreateInfo createInfo = {
      VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,

  };

  return NULL;
}

void initVulkan() { createInstance(); }

void loop(GLFWwindow *window) {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}
