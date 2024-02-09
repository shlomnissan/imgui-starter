// Copyright 2024 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <vector>

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/framebuffer.h"
#include "core/mesh.h"
#include "core/shader.h"
#include "core/window.h"

#include "shaders/headers/vertex.h"
#include "shaders/headers/fragment.h"

auto main() -> int {
    const auto width = 1024;
    const auto height = 768;
    const auto settings_width = 220;
    const auto viewport_width = width - settings_width;
    const auto viewport_height = height;

    auto window = Window {width, height, "ImGui starter project"};

    auto shader = Shader {{
        {ShaderType::kVertexShader, _SHADER_vertex},
        {ShaderType::kFragmentShader, _SHADER_fragment}
    }};
    
    glViewport(0, 0, viewport_width, viewport_height);
    glEnable(GL_DEPTH_TEST);

    auto updateProjection = [&shader](int width, int height) {
        auto ratio = static_cast<float>(width) / static_cast<float>(height);
        shader.SetMat4("Projection", glm::perspective(45.0f, ratio, 0.1f, 100.0f));
    };
    updateProjection(viewport_width, viewport_height);

    auto framebuffer = Framebuffer {viewport_width, viewport_height};
    auto color_0 = std::vector<float> {1.0f, 0.0f, 0.0f, 1.0f};
    auto color_1 = std::vector<float> {0.0f, 1.0f, 0.0f, 1.0f};
    auto color_2 = std::vector<float> {0.0f, 0.0f, 1.0f, 1.0f};

    const auto RenderSettings = [&]() {
        ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(0x80, 0x80, 0xFF, 0x8C));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(0x80, 0x80, 0xFF, 0x8C));
        ImGui::Begin("Color Settings", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize
        );
        ImGui::SetWindowSize({settings_width, height});
        ImGui::SetWindowPos({0, 0});
        ImGui::ColorEdit3("Vertex 0", color_0.data());
        ImGui::ColorEdit3("Vertex 1", color_1.data());
        ImGui::ColorEdit3("Vertex 2", color_2.data());
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
        ImGui::End();
    };

    const auto RenderViewport = [&]() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
        ImGui::Begin("Perspective View", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove
        );
        ImGui::SetWindowPos({settings_width, 0});
        ImGui::SetWindowSize({viewport_width, viewport_height});
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            (void *)framebuffer.texture_id(),
            ImVec2(pos.x, pos.y), 
            ImVec2(pos.x + framebuffer.width(), pos.y + framebuffer.height()), 
            ImVec2(0, 1), ImVec2(1, 0)
        );
        ImGui::End();
        ImGui::PopStyleVar();
    };

    window.Start([&](const double delta){
        auto triangle = Mesh({
            -0.5f, -0.5f, -2.0f, color_0[0], color_0[1], color_0[2],
            0.0f,  0.5f, -2.0f, color_1[0], color_1[1], color_1[2],
            0.5f, -0.5f, -2.0f, color_2[0], color_2[1], color_2[2],
        });

        framebuffer.Bind();
        glClearColor(0.17f, 0.16f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        triangle.Draw(shader);
        framebuffer.Unbind();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderSettings();
        RenderViewport();
    });

    return 0;
}