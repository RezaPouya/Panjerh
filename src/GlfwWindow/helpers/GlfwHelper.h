#pragma once
#include <iostream>
#include <stdexcept>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <functional>
#include <algorithm>

class GlfwHelper
{
public:
    GlfwHelper(const char* title) ;
    ~GlfwHelper();
    void RenderLoop(const std::function<void()>& func);
    GLFWwindow* GetWindow();

private:
    const char* m_winTitle;
    GLFWwindow* glfwWindow = nullptr;
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
