#include <iostream>
#include <chrono>
#include <thread>

#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

using namespace std;

static void glfw_error_callback(int error, const char* description);
static const char* decide_opengl_glsl_versions(void);

int main()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }

    const char* glsl_version = decide_opengl_glsl_versions();

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW + OpenGL3 + Conan + CMake Setup", nullptr, nullptr);
    if (!window) {
        cerr << "error: glfw: failed to create window\n";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);

    // Main loop
    constexpr float desired_fps = 60.0f;
    constexpr float desired_frame_rate_ms = 1000.0f / desired_fps;
    while (!glfwWindowShouldClose(window)) {
        auto frame_start = chrono::high_resolution_clock::now();

        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            int window_width, window_height;
            glfwGetWindowSize(window, &window_width, &window_height);

            int margin = 10;
            ImGui::SetNextWindowPos(ImVec2(0.0f + margin, 0.0f + margin));
            ImGui::SetNextWindowSize(ImVec2(window_width - 2*margin, window_height - 2*margin));

            ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoCollapse/* | ImGuiWindowFlags_NoTitleBar*/);
                ImGui::Text("Hello, World!");
                // float frame_rate_ms = 1000.0f / io.Framerate;
                // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", frame_rate_ms, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Simple FPS control.
        // If rendering was too fast and frame duration was too low compared to the desired frame rate, then
        // slow the rendering down by its difference
        float frame_duration_ms = io.DeltaTime / 1000.f;
        if (frame_duration_ms < desired_frame_rate_ms) {
            float delay = desired_frame_rate_ms - frame_duration_ms;
            this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(delay)));
        }

        glfwSwapBuffers(window);
    }

    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // glfw cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "error: glfw: [%d] %s\n", error, description);
}

/**
 * Decide GL+GLSL versions
 */
static const char* decide_opengl_glsl_versions(void)
{
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    static const char *const glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    static const char *const glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    static const char *const glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    return glsl_version;
}
