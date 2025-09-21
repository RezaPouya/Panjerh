#include "FrameUtility.h"
#include <stdexcept>

const MonitorInfo FrameUtility::GetPrimaryMonitorInfo() {

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    
    if (!primaryMonitor) {
        throw std::runtime_error("Failed to get primary monitor");
    }

    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    
    if (!videoMode) {
        throw std::runtime_error("Failed to get video mode");
    }

    float xscale, yscale;
    glfwGetMonitorContentScale(primaryMonitor, &xscale, &yscale);

    return MonitorInfo{
        .Width = static_cast<uint16_t>(videoMode->width),
        .Height = static_cast<uint16_t>(videoMode->height),
        .RefreshRate = static_cast<uint16_t>(videoMode->refreshRate),
        .ContentScale = (xscale + yscale) / 2.0f,
        .Name = glfwGetMonitorName(primaryMonitor)
    };
}

const WindowConfig FrameUtility::GetDefaultConfig() {
    return WindowConfig{
        .Width = 1280,
        .Height = 800,
        .Title = "Dear ImGui Application",
        .FullScreen = false,
        .VSync = true
    };
}

const WindowConfig FrameUtility::GetFullScreenConfig() {

    MonitorInfo monitorInfo = FrameUtility::GetPrimaryMonitorInfo();

    WindowConfig winConfig
    {
        .Width = monitorInfo.Width,
        .Height = monitorInfo.Height,
        .Title = "Dear ImGui Application",
        .FullScreen = true,
        .VSync = true
    };

    return  winConfig;
}

void FrameUtility::CenterWindowOnMonitor(GLFWwindow* window, const MonitorInfo& monitorInfo) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetWindowPos(
        window,
        (monitorInfo.Width - static_cast<uint16_t>(windowWidth)) / 2,
        (monitorInfo.Height - static_cast<uint16_t>(windowHeight)) / 2
    );
}

void FrameUtility::ToggleFullScreen(GLFWwindow* window, WindowConfig& config, const MonitorInfo& monitorInfo) {

    if (config.FullScreen) {
        // Switch to windowed
        glfwSetWindowMonitor(
            window,
            nullptr,
            100, 100,
            config.Width,
            config.Height,
            GLFW_DONT_CARE
        );
        config.FullScreen = false;
    }
    else {
        // Switch to fullscreen
        glfwSetWindowMonitor(
            window,
            glfwGetPrimaryMonitor(),
            0, 0,
            monitorInfo.Width,
            monitorInfo.Height,
            monitorInfo.RefreshRate
        );
        config.FullScreen = true;
    }
}