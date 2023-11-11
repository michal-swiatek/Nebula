//
// Created by michal-swiatek on 08.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "debug/ImGuiLayer.h"

#include <array>
#include <format>

#include <imgui.h>
#include <numeric>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "core/Application.h"
#include "core/Timestep.h"
#include "platform/OpenGL/OpenGLConfiguration.h"

namespace nebula {

    int ImGuiLayer::s_counter = 0;

    ImGuiLayer::ImGuiLayer() : Layer(std::format("ImGuiLayer{}", s_counter++)) {}

    void ImGuiLayer::onAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        auto& application = Application::get();
        auto* window = static_cast<GLFWwindow*>(application.getWindow().getWindowHandle());

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(OPENGL_VERSION_STRING.c_str());
    }

    void ImGuiLayer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::onEvent(Event& event)
    {
        if (m_block_events)
        {
            ImGuiIO& io = ImGui::GetIO();
            event.handled |= event.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            event.handled |= event.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

    void ImGuiLayer::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::onImGuiRender()
    {
        if (!m_enabled)
            return;

        ImGui::ShowDemoWindow();

        performanceOverlay();
    }

    void ImGuiLayer::performanceOverlay()
    {
        ImGui::Begin("Performance");

        fpsSection();

        ImGui::End();
    }

    void ImGuiLayer::fpsSection()
    {
        //  Frame time graph
        static constexpr int n_frames = 100;
        static std::array<float, n_frames> frame_times{};
        static int frame_offset = 0;

        //  FPS control variables
        static bool vsync = Application::get().getWindow().getProperties().vsync;
        static int update_fps = Application::get().getUpdateFps();
        static int render_fps = Application::get().getRenderFps();

        //  Update current frame time info
        auto frame_time = Timestep(m_frame_timer.elapsedSeconds(true));
        auto frame_milliseconds = frame_time.getMilliSeconds();

        int current_fps = 1.0 / frame_time;
        int target_update_fps = Application::get().getUpdateFps();
        int target_render_fps = Application::get().getRenderFps();
        float target_milliseconds = target_render_fps == 0 ? 0.0f : 1000.0f / target_render_fps;

        //  Update current application settings
        auto& application = Application::get();

        vsync = application.getWindow().getProperties().vsync;
        update_fps = application.getUpdateFps();
        render_fps = application.getRenderFps();

        //  Update graph info
        frame_times[frame_offset] = static_cast<float>(frame_milliseconds);
        frame_offset = (frame_offset + 1) % n_frames;

        int average_fps = 1000.0f * n_frames / std::accumulate(frame_times.begin(), frame_times.end(), 0.0f);
        int one_percent_low = 1000.0f / *std::ranges::max_element(frame_times);

        //  Prepare text
        auto fps_text = target_render_fps == 0 ? "unlimited" : std::to_string(target_render_fps);
        auto average_fps_text = std::format("avg fps: {}, 1% low: {}", average_fps, one_percent_low);
        auto text_color = (float)current_fps / target_render_fps > 0.95f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

        if (ImGui::CollapsingHeader("Frames per second", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("VSync", &vsync);
            ImGui::SliderInt("Update fps", &update_fps, 1, 200);
            ImGui::SliderInt("Render fps", &render_fps, 0, 500);

            ImGui::Separator();

            ImGui::Text("Target update fps: %i (%.3fms)", target_update_fps, 1000.0f / target_update_fps);
            ImGui::Text("Target render fps: %s (%.3fms)", fps_text.c_str(), target_milliseconds);

            ImGui::TextColored(text_color, "Current fps: %i", current_fps);
            ImGui::TextColored(text_color, "Current frame time: %ims", static_cast<int>(frame_milliseconds));

            ImGui::Separator();

            ImGui::PlotLines("Frame time [ms]", frame_times.data(), n_frames, frame_offset, average_fps_text.c_str(), 0.0f, 50.0f, ImVec2(0, 80.0f));
        }

        //  Set new fps settings
        application.setUpdateFps(update_fps);
        application.setRenderFps(render_fps);
        application.getWindow().setVSync(vsync);
    }

}
