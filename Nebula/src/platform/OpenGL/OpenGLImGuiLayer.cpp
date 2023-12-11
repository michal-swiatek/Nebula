//
// Created by michal-swiatek on 11.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLImGuiLayer.h"

#include <imgui.h>

#include "rendering/RenderContext.h"

using namespace nebula::rendering;

namespace nebula {

    OpenGLImGuiLayer::OpenGLImGuiLayer(RenderPass& renderpass) : ImGuiLayer(renderpass) {}

    void OpenGLImGuiLayer::apiSection()
    {
        static ApiInfo s_api_info = RenderContext::get().getApiInfo();

        if (ImGui::CollapsingHeader("Rendering API", ImGuiTreeNodeFlags_DefaultOpen))
        {
            #ifdef NB_DEBUG_BUILD
            ImGui::Text("Api: %s, build: Debug", s_api_info.api_name.c_str());
            #else
            ImGui::Text("Api: %s, build: Release", s_api_info.api_name.c_str());
            #endif
            ImGui::Text("Vendor: %s", s_api_info.vendor_name.c_str());
            ImGui::Text("Renderer: %s", s_api_info.renderer_name.c_str());
            ImGui::Text("Version: %s", s_api_info.driver_version.c_str());
            ImGui::NewLine();

        }
    }

}
