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
     return false;
}

void Engine::createInstance() {
     VkApplicationInfo appInfo{};
     appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
     appInfo.pApplicationName   = "Meglinge";
     appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
     appInfo.pEngineName        = "Aruze Radiation Engine";
     appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
     appInfo.apiVersion         = VK_API_VERSION_1_0;

     VkInstanceCreateInfo createInfo{};
     createInfo.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
     createInfo.pApplicationInfo     = &appInfo;

     uint32_t     glfwExtensionCount = 0;
     const char** glfwExtensions;
     glfwExtensions                     = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

     createInfo.enabledExtensionCount   = glfwExtensionCount;
     createInfo.ppEnabledExtensionNames = glfwExtensions;
     createInfo.enabledLayerCount       = 0;

     uint32_t extensionCount            = 0;
     vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
     std::vector<VkExtensionProperties> extensions(extensionCount);
     vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
     spdlog::info("available extensions:");
     for (const auto& extension : extensions) {
          spdlog::info("\t{}", extension.extensionName);
     }
     if (vkCreateInstance(&createInfo, nullptr, &instance)) {
          spdlog::error("Failed to create instance!");
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