#include "ImGuiWrapper.h"
#include <memory>
#include <stdio.h>

#define GL_SILENCE_DEPRECATION

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

// Win32 specific
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

ImGuiWrapper::ImGuiWrapper(int width, int height, const std::string& title)
    : window(nullptr), window_width(width), window_height(height), window_title(title),
    clear_color(ImVec4(0.45f, 0.55f, 0.60f, 1.00f)), main_scale(1.0f) {
    glfwSetErrorCallback(glfw_error_callback);
}

ImGuiWrapper::~ImGuiWrapper() {
    cleanup();
}

bool ImGuiWrapper::initialize() {
    if (!glfwInit()) {
        return false;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    // Get content scale for DPI awareness
    main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    // Create window
    window = glfwCreateWindow(
        static_cast<int>(window_width * main_scale),
        static_cast<int>(window_height * main_scale),
        window_title.c_str(), nullptr, nullptr
    );

    if (window == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    setupImGui();
    return true;
}

void ImGuiWrapper::setupImGui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;
#endif

    // When viewports are enabled
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    loadFonts();
}

void ImGuiWrapper::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    // Note: You might want to make font loading configurable
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 16.0f * main_scale);
    // io.Fonts->AddFontFromFileTTF("imgui/misc/fonts/Roboto-Medium.ttf", 16.0f * main_scale);
}

void ImGuiWrapper::run() {
    while (!shouldClose()) {
        beginFrame();
        // Your application logic goes here (to be implemented by user)
        endFrame();
        render();
    }
}

bool ImGuiWrapper::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void ImGuiWrapper::beginFrame() {
    glfwPollEvents();

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWrapper::endFrame() {
    ImGui::EndFrame();
}

void ImGuiWrapper::render() {
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(
        clear_color.x * clear_color.w,
        clear_color.y * clear_color.w,
        clear_color.z * clear_color.w,
        clear_color.w
    );
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(window);
}

void ImGuiWrapper::cleanup() {
    if (window) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
        window = nullptr;
    }
}

float ImGuiWrapper::getFramerate() const { return ImGui::GetIO().Framerate; }

void ImGuiWrapper::ShowSimpleExampleWindow() {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");
    ImGui::Text("This is some useful text.");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&getClearColor());

    if (ImGui::Button("Button"))
        counter++;

    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / getFramerate(), getFramerate());
    ImGui::End();
}