// Copyright 2024 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <vector>

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/mesh.h"
#include "core/shader.h"
#include "core/window.h"

#include "shaders/headers/vertex.h"
#include "shaders/headers/fragment.h"

auto main() -> int {
    auto window = Window {800, 600, "ImGui Starter"};

    auto shader = Shader {{
        {ShaderType::kVertexShader, _SHADER_vertex},
        {ShaderType::kFragmentShader, _SHADER_fragment}
    }};
    
    glEnable(GL_DEPTH_TEST);

    auto color_0 = std::vector<float> {1.0f, 0.0f, 0.0f, 1.0f};
    auto color_1 = std::vector<float> {0.0f, 1.0f, 0.0f, 1.0f};
    auto color_2 = std::vector<float> {0.0f, 0.0f, 1.0f, 1.0f};

    auto ratio = static_cast<float>(800 / 600);

    shader.Use();
    shader.SetMat4("Projection", glm::perspective(45.0f, ratio, 0.1f, 100.0f));

    window.Start([&](const double delta){
        auto triangle = Mesh({
            -0.5f, -0.5f, -2.0f, color_0[0], color_0[1], color_0[2],
            0.0f,  0.5f, -2.0f, color_1[0], color_1[1], color_1[2],
            0.5f, -0.5f, -2.0f, color_2[0], color_2[1], color_2[2],
        });

        glClearColor(0.17f, 0.16f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Begin("Color", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar
        );

        ImGui::SetWindowSize({220, 82});
        ImGui::SetWindowPos({20, 20});
        ImGui::ColorEdit3("Vertex 0", color_0.data());
        ImGui::ColorEdit3("Vertex 1", color_1.data());
        ImGui::ColorEdit3("Vertex 2", color_2.data());
        ImGui::End();

        triangle.Draw(shader);
    });

    return 0;
}