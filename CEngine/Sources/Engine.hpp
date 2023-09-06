class Engine {
public:
     void run();

private:
     void                     initWindow();
     void                     createInstance();
     void                     initVulkan();
     void                     mainLoop();
     void                     cleanup();
     bool                     checkValidationLayerSupport();
     std::vector<const char*> getRequiredExtensions();
     void                     setupDebugMessenger();
     VkResult                 CreateDebugUtilsMessengerEXT(
                         VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                         VkDebugUtilsMessengerEXT* pDebugMessenger
                     );
     void DestroyDebugUtilsMessengerEXT(
         VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator
     );
     void                                  populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
     static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
         VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData
     ) {
          if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
               spdlog::error("validation layer: {}", pCallbackData->pMessage);
          } else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
               spdlog::warn("validation layer: {}", pCallbackData->pMessage);
          } else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
               spdlog::info("validation layer: {}", pCallbackData->pMessage);
          } else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
               spdlog::info("validation layer: {}", pCallbackData->pMessage);
          } else {
               spdlog::debug("validation layer: {}", pCallbackData->pMessage);
          }

          
          
          return VK_FALSE;
     }
     GLFWwindow*              window;
     VkInstance               instance;
     VkDebugUtilsMessengerEXT debugMessenger;
};