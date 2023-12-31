#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>
#include "Engine.hpp"
const uint32_t WIDTH  = 800;
const uint32_t HEIGHT = 600;

//

const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const bool                     enableValidationLayers = true;

//#ifdef NDEBUG
//const bool enableValidationLayers = false;
//#else
//const bool enableValidationLayers = true;
//#endif

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
void Engine::setupDebugMessenger() {
     if (!enableValidationLayers) return;
     VkDebugUtilsMessengerCreateInfoEXT createInfo;
     populateDebugMessengerCreateInfo(createInfo);

     if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
          spdlog::error("failed to set up debug messenger!");
     }
}
VkResult Engine::CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
) {
     auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
     if (func != nullptr) {
          return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
     } else {
          return VK_ERROR_EXTENSION_NOT_PRESENT;
     }
}
void Engine::DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator
) {
     auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
     if (func != nullptr) {
          func(instance, debugMessenger, pAllocator);
     }
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
     VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
     if (enableValidationLayers) {
          createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
          createInfo.ppEnabledLayerNames = validationLayers.data();

          populateDebugMessengerCreateInfo(debugCreateInfo);
          createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
     } else {
          createInfo.enabledLayerCount = 0;
     }
     if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
          throw std::runtime_error("failed to create instance!");
     }
}
void Engine::initVulkan() {
     createInstance();
     setupDebugMessenger();
}
void Engine::mainLoop() {
     while (!glfwWindowShouldClose(window)) {
          glfwPollEvents();
     }
}
void Engine::cleanup() {
     if (enableValidationLayers) {
          DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
     }
     vkDestroyInstance(instance, nullptr);
     glfwDestroyWindow(window);
     glfwTerminate();
}
void Engine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
     createInfo                 = {};
     createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
     createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
     createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
     createInfo.pfnUserCallback = debugCallback;
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