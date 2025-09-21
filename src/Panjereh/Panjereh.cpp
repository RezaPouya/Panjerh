#include <memory>
#include "imgui/my_wrappers/ImGuiWrapper.h"

int main() {

    auto imguiApp = std::make_unique<ImGuiWrapper>();

    if (!imguiApp->initialize()) {
        return 1;
    }

    // Main loop - now very simple!
    while (!imguiApp->shouldClose()) {

        imguiApp->beginFrame();

        // Your application logic here
        imguiApp->ShowSimpleExampleWindow();

        imguiApp->endFrame();
        imguiApp->render();
    }

    return 0;
}