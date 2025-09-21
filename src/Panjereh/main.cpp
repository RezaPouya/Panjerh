#include <memory>
#include "imgui/my_wrappers/ImGuiWrapper.h"
#include "Utilities/FrameUtility.h"
#include <stdexcept>

int main() {
	// First create the wrapper with default constructor
	// (it will handle GLFW initialization internally)
	WindowConfig config{
		   .Width = 1024,  // Default values as fallback
		   .Height = 768,
		   .Title = "Dear ImGui Application",
		   .FullScreen = false,
		   .VSync = true,
	};

	MonitorInfo monitorInfo{
		.Width = 1920,
		.Height = 1080,
		.RefreshRate = 30,
		.ContentScale = 1.0f,
		.Name = "Primary Monitor",
	};

	// Create ImGui wrapper with temporary config
	ImGuiWrapper imguiApp(config, monitorInfo);

	if (!imguiApp.Initialize()) {
		return 1;
	}

	// Now that GLFW is initialized, we can get the real monitor info
	try {
		MonitorInfo realMonitorInfo = FrameUtility::GetPrimaryMonitorInfo();
		WindowConfig realConfig = FrameUtility::GetFullScreenConfig();

		// Optionally update the window to use real monitor settings
		if (imguiApp.IsFullScreen()) {
			imguiApp.ToggleFullScreen(); // Refresh with real settings
		}
	}
	catch (const std::runtime_error& e) {
		// Handle error gracefully - continue with default values
		printf("Warning: Failed to get monitor info: %s\n", e.what());
	}

	// Main loop
	while (!imguiApp.ShouldClose()) {
		imguiApp.BeginFrame();
		// ------------------------------------------------



		// ------------------------------------------------
		imguiApp.EndFrame();
		imguiApp.Render();
	}

	return 0;
}