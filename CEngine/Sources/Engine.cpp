#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include "Engine.hpp"
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

//

const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

//
void Engine::run() {
     initWindow();
     initVulkan();
     mainLoop();
     cleanup();
}

void Engine::initWindow() {
     glfwInit();
     glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
     glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
     window = glfwCreateWindow(WIDTH, HEIGHT, "vulkan", nullptr, nullptr);
}

bool Engine::checkValidationLayerSupport() {
     uint32_t layerCount;
     vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
     std::vector<VkLayerProperties> availableLayers(layerCount);
     vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
     for (const char* layerName : validationLayers) {
          bool layerFound = false;

          for (const auto& layerProperties : availableLayers) {
               if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
               }
          }

          if (!layerFound) {
               return false;
          }
     }
     return true;
}

std::vector<const char*> Engine::getRequiredExtensions() {
     uint32_t     glfwExtensionCount = 0;
     const char** glfwExtensions;
     glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
     std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
     if (enableValidationLayers) {
          extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
     }
     return extensions;
}

void Engine::createInstance() {
     if (enableValidationLayers && !checkValidationLayerSupport()) {
          spdlog::error("validation layers requested, but not available!");
     }

     VkApplicationInfo appInfo{};
     appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
     appInfo.pApplicationName   = "Meglinge";
     appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
     appInfo.pEngineName        = "Aruze Radiation Engine";
     appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
     appInfo.apiVersion         = VK_API_VERSION_1_0;

     VkInstanceCreateInfo createInfo{};
     createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
     createInfo.pApplicationInfo        = &appInfo;

     auto extensions                    = getRequiredExtensions();
     createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
     createInfo.ppEnabledExtensionNames = extensions.data();
     if (enableValidationLayers) {
          createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
          createInfo.ppEnabledLayerNames = validationLayers.data();
     } else {
          createInfo.enabledLayerCount = 0;
     }
}
void Engine::initVulkan() {
     createInstance();
}
void Engine::mainLoop() {
     while (!glfwWindowShouldClose(window)) {
          glfwPollEvents();
     }
}
void Engine::cleanup() {
     vkDestroyInstance(instance, nullptr);
     glfwDestroyWindow(window);
     glfwTerminate();
}

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