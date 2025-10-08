// GlfwHelper.cpp
#include "GlfwHelper.h"

GlfwHelper::GlfwHelper(const char* winTitle) : glfwWindow(nullptr) , m_winTitle(winTitle)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindow = glfwCreateWindow(800, 800, m_winTitle, nullptr, nullptr);

    if (!glfwWindow) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // In GlfwHelper constructor, after creating window:
    glfwMakeContextCurrent(glfwWindow);

    // Initialize GLAD
    if (!gladLoadGL(glfwGetProcAddress)) {
        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Set the viewport initially
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    glViewport(0, 0, width, height);
}

GlfwHelper::~GlfwHelper()
{
    if (glfwWindow) {
        glfwDestroyWindow(glfwWindow);
    }
    glfwTerminate();
}

void GlfwHelper::RenderLoop(const std::function<void()>& renderFunc) {
    if (!glfwWindow || !renderFunc) {
        std::cout << "Invalid window or render function!" << std::endl;
        return;
    }

    while (!glfwWindowShouldClose(glfwWindow)) {
        int width, height;
        glfwGetFramebufferSize(glfwWindow, &width, &height);

        // CRITICAL: Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // CRITICAL: Set viewport
        glViewport(0, 0, width, height);

        // Call the render function
        renderFunc();

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
}

void GlfwHelper::error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void GlfwHelper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void GlfwHelper::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


GLFWwindow* GlfwHelper::GetWindow()
{
    return this->glfwWindow;
}