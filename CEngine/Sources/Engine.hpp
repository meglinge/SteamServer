class Engine {
public:
     void run();

private:
     void                                  initWindow();
     void                                  createInstance();
     void                                  initVulkan();
     void                                  mainLoop();
     void                                  cleanup();
     bool                                  checkValidationLayerSupport();
     std::vector<const char*>              getRequiredExtensions();
     static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
         VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType,
         const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData
     );
     GLFWwindow* window;
     VkInstance  instance;
};