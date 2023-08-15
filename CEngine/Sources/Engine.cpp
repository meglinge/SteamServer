#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;
class Engine {
public:
     void run() {
          initWindow();
          initVulkan();
          mainLoop();
          cleanup();
     }

private:
     GLFWwindow* window;

     void        initWindow() {
          glfwInit();
          glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
          glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
          window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan", nullptr, nullptr);
     }
     void initVulkan() {}
     void mainLoop() {
          while (!glfwWindowShouldClose(window)) {
               glfwPollEvents();
          }
     }
     void cleanup() {
          glfwDestroyWindow(window);
          glfwTerminate();
     }
};

int main() {
     Engine app;
     try {
          app.run();
     } catch (const std::exception& e) {
          spdlog::error("{}", e.what());
          return EXIT_FAILURE;
     }
     return EXIT_SUCCESS;
}