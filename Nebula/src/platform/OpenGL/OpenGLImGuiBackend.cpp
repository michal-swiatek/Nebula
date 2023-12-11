//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLImGuiBackend.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "core/Application.h"
#include "platform/OpenGL/OpenGLConfiguration.h"

using namespace nebula::rendering;

namespace nebula {

    OpenGlImGuiBackend::OpenGlImGuiBackend(RenderPass& renderpass) : m_renderpass(renderpass)
    {

    }

    void OpenGlImGuiBackend::onAttach()
    {
        auto* window = static_cast<GLFWwindow*>(Application::getWindow().getWindowHandle());
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplOpenGL3_Init(GLSL_VERSION_STRING.c_str());
    }

    void OpenGlImGuiBackend::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
    }

    void OpenGlImGuiBackend::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void OpenGlImGuiBackend::end(void* command_buffer_handle)
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void OpenGlImGuiBackend::init()
    {

    }

    void OpenGlImGuiBackend::shutdown()
    {

    }


}
