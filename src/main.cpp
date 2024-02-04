// Copyright 2024 Betamark Pty Ltd. All rights reserved.
// Author: Shlomi Nissan (shlomi@betamark.com)

#include <vector>

#include <imgui.h>
#include <glm/glm.hpp>

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

    auto triangle = Mesh({
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    });

    auto color = std::vector<float> {1.0f, 1.0f, 1.0f, 1.0f};

    window.Start([&](const double delta){
        glClearColor(0.17f, 0.16f, 0.29f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Begin("Color", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar
        );

        ImGui::SetWindowSize({250, 35});
        ImGui::SetWindowPos({20, 20});
        ImGui::ColorEdit3("Edit Color", color.data());
        ImGui::End();

        triangle.Draw(shader);
    });

    return 0;
}