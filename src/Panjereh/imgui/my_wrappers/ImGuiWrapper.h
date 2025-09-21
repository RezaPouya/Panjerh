#pragma once

#include "../imgui.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

class ImGuiWrapper {
public:
    ImGuiWrapper(int width = 1280, int height = 800, const std::string& title = "Dear ImGui Application");
    ~ImGuiWrapper();

    // Delete copy constructor and assignment operator
    ImGuiWrapper(const ImGuiWrapper&) = delete;
    ImGuiWrapper& operator=(const ImGuiWrapper&) = delete;

    bool initialize();
    void run();
    bool shouldClose() const;
    void beginFrame();
    void endFrame();
    void render();


    // example
    void ShowSimpleExampleWindow();
    
    
    // Public access to commonly used objects 
    float getFramerate() const;
    GLFWwindow* getWindow() const { return window; }
    ImGuiIO& getIO() { return ImGui::GetIO(); } // Return reference, not pointer
    ImVec4& getClearColor() { return clear_color; }

private:
    void setupImGui();
    void loadFonts();
    void cleanup();

    GLFWwindow* window;
    std::string glsl_version;
    ImVec4 clear_color;
    int window_width;
    int window_height;
    std::string window_title;
    float main_scale;
};

