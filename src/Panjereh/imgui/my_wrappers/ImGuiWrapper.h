#pragma once
#include <memory>
#include <string>
#include <cstdint>
#include <GLFW/glfw3.h>

#include "../imgui.h"
#include "../backends/imgui_impl_glfw.h"
#include "../backends/imgui_impl_opengl3.h"
#include "../../Utilities/FrameUtility.h"

class ImGuiWrapper {
public:
    // Constructor takes WindowConfig and optional MonitorInfo
    ImGuiWrapper(const WindowConfig& config,
        const MonitorInfo& monitorInfo = FrameUtility::GetPrimaryMonitorInfo());

    ~ImGuiWrapper();

    // Delete copy constructor and assignment operator
    ImGuiWrapper(const ImGuiWrapper&) = delete;
    ImGuiWrapper& operator=(const ImGuiWrapper&) = delete;

    bool Initialize();
    void Run();
    bool ShouldClose() const;
    void BeginFrame();
    void EndFrame();
    void Render();

    // Fullscreen control
    void ToggleFullScreen();
    bool IsFullScreen() const;

    // show example 
    void ShowSimpleExampleWindow();

    // Public access
    GLFWwindow* GetWindow() const { return window; }
    ImGuiIO& GetIO() { return ImGui::GetIO(); }
    ImVec4& GetClearColor() { return clear_color; }
    const WindowConfig& GetConfig() const { return config; }
    const MonitorInfo& GetMonitorInfo() const { return monitorInfo; }

private:
    void SetupImGui();
    void LoadFonts();
    void Cleanup();

    GLFWwindow* window;
    std::string glsl_version;
    ImVec4 clear_color;
    WindowConfig config;
    MonitorInfo monitorInfo;
    float main_scale;
};
