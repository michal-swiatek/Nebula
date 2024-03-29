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

#include "core/Timestep.h"
#include "core/Application.h"
#include "core/UpdateContext.h"
#include "rendering/RenderContext.h"

#include "platform/Vulkan/VulkanImGuiBackend.h"
#include "platform/OpenGL/OpenGLImGuiBackend.h"

using namespace nebula::rendering;

namespace nebula {

    int ImGuiLayer::s_counter = 0;
    ImGuiBackend* ImGuiLayer::s_backend = nullptr;

    void ImGuiBackend::init()
    {
        switch (Application::get().getRenderingAPI())
        {
            case API::cVulkan:  VulkanImGuiBackend::init();     break;
            case API::cOpenGL:  OpenGlImGuiBackend::init();     break;
            default:    NB_CORE_ASSERT(false, "Unsupported rendering API!");
        }
    }

    void ImGuiBackend::shutdown()
    {
        switch (Application::get().getRenderingAPI())
        {
            case API::cVulkan:  VulkanImGuiBackend::shutdown();     break;
            case API::cOpenGL:  OpenGlImGuiBackend::shutdown();     break;
            default:    NB_CORE_ASSERT(false, "Unsupported rendering API!");
        }
    }

    ImGuiLayer::ImGuiLayer(RenderPass& renderpass) : Layer(std::format("ImGuiLayer{}", s_counter++))
    {
        NB_CORE_ASSERT(!s_backend);
        m_backend = ImGuiBackend::create(renderpass);
        s_backend = m_backend.get();
    }

    ImGuiLayer::~ImGuiLayer()
    {
        NB_CORE_ASSERT(s_backend);
        s_backend = nullptr;
    }

    void ImGuiLayer::onAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        s_backend->onAttach();
    }

    void ImGuiLayer::onDetach()
    {
        s_backend->onDetach();
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
        s_backend->begin();

        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::end(void* command_buffer_handle)
    {
        ImGuiIO& io = ImGui::GetIO();
        const Window& window = Application::getWindow();
        io.DisplaySize = ImVec2(static_cast<float>(window.getWidth()), static_cast<float>(window.getHeight()));

        s_backend->end(command_buffer_handle);

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::reloadBackend(RenderPass& renderpass)
    {
        NB_CORE_ASSERT(s_backend);
        m_backend = ImGuiBackend::create(renderpass);
        s_backend = m_backend.get();
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

        apiSection();
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
        static bool vsync = RenderContext::get().checkVSync();
        static float update_timestep = UpdateContext::get().getUpdateTimestep();
        static int render_fps = RenderContext::get().getRenderFps();

        //  Update current application settings
        auto& application = Application::get();
        auto& update_context = UpdateContext::get();
        auto& render_context = RenderContext::get();

        // vsync = application.getWindow().getProperties().vsync;
        vsync = render_context.checkVSync();
        update_timestep = update_context.getUpdateTimestep();
        render_fps = render_context.getRenderFps();

        //  Update current frame time info
        auto frame_time = Timestep(m_frame_timer.elapsedSeconds(true));
        auto frame_milliseconds = frame_time.getMilliSeconds();

        int current_fps = 1.0 / frame_time;
        float target_milliseconds = render_fps == 0 ? 0.0f : 1000.0f / render_fps;

        //  Update graph info
        frame_times[frame_offset] = static_cast<float>(frame_milliseconds);
        frame_offset = (frame_offset + 1) % n_frames;

        int average_fps = 1000.0f * n_frames / std::accumulate(frame_times.begin(), frame_times.end(), 0.0f);
        int one_percent_low = 1000.0f / *std::ranges::max_element(frame_times);

        //  Prepare text
        auto fps_text = render_fps == 0 ? "unlimited" : std::to_string(render_fps);
        auto average_fps_text = std::format("avg fps: {}, 1% low: {}", average_fps, one_percent_low);
        auto text_color = (float)current_fps / render_fps > 0.9f ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

        if (ImGui::CollapsingHeader("Frames per second", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("VSync", &vsync);
            ImGui::SliderFloat("Update timestep", &update_timestep, 0.01, 1.0);
            ImGui::SliderInt("Render fps", &render_fps, 0, 500);

            ImGui::Separator();

            ImGui::Text("Fixed update timestep: %.3fs", update_timestep);
            ImGui::Text("Target render fps: %s (%.3fms)", fps_text.c_str(), target_milliseconds);

            ImGui::TextColored(text_color, "Current fps: %i", current_fps);
            ImGui::TextColored(text_color, "Current frame time: %.3fms", frame_milliseconds);

            ImGui::Separator();

            ImGui::PlotLines("Frame time [ms]", frame_times.data(), n_frames, frame_offset, average_fps_text.c_str(), 0.0f, 50.0f, ImVec2(0, 80.0f));
        }

        //  Set new fps settings
        update_context.setUpdateTimestep(update_timestep);
        render_context.setRenderFps(render_fps);
        render_context.setVSync(vsync);
    }

}
