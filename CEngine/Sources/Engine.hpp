class Engine {
public:
     void run();

private:
     void        initWindow();
     void        createInstance();
     void        initVulkan();
     void        mainLoop();
     void        cleanup();
     bool        checkValidationLayerSupport();
     GLFWwindow* window;
     VkInstance  instance;
};