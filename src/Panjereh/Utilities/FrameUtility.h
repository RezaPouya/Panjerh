#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <cstdint> // For uint16_t

struct MonitorInfo {
    uint16_t Width;
    uint16_t Height;
    uint16_t RefreshRate;
    float ContentScale;
    std::string Name;
};

struct WindowConfig {
    uint16_t Width;
    uint16_t Height;
    std::string Title;
    bool FullScreen;
    bool VSync;
};

class FrameUtility {
public:
    const static MonitorInfo GetPrimaryMonitorInfo();
    const static WindowConfig GetDefaultConfig();
    const static WindowConfig GetFullScreenConfig();

    static void CenterWindowOnMonitor(GLFWwindow* window, const MonitorInfo& monitorInfo);
    static void ToggleFullScreen(GLFWwindow* window, WindowConfig& config, const MonitorInfo& monitorInfo);

private:
    FrameUtility() = delete; // Static class, no instances
};