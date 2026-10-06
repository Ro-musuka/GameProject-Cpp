#include "Editor.h"

#include <algorithm>

#include <SDL3/SDL.h>
#include <GL/glew.h>

#include "Graphics/Renderer.h"

Editor::Editor(Renderer *renderer)
    : mRenderer(renderer)
{
    mScreenWidth = mRenderer->GetScreenWidth();
    mScreenHeight = mRenderer->GetScreenHeight();
}

Editor::~Editor()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();

    ImGui::DestroyContext();
}

bool Editor::Init(const std::string &openGL_version)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForOpenGL(mRenderer->GetWindow(), mRenderer->GetContext()))
    {
        SDL_Log("Dear ImGuiの初期化に失敗しました。\nImGui_ImplSDL3_InitForOpenGL()のエラー");
        return false;
    }
    if (!ImGui_ImplOpenGL3_Init(openGL_version.c_str()))
    {
        SDL_Log("Dear ImGuiの初期化に失敗しました。\nImGui_ImplOpenGL3_Init()のエラー");
        return false;
    }

    return true;
}

void Editor::ProcessEvent(const SDL_Event &event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}

void Editor::Draw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();

    // ウィンドウ作成
    ImGui::Begin(" ", nullptr);
    bool temp = true;
    ImGui::Checkbox("Demo Window", &temp);
    float f = 0.5f;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::ColorEdit3("clear color", (float *)&clear_color);

    ImGui::Button("Play");
    // 同じ行に描画
    ImGui::SameLine();

    ImGui::Button("Pause");

    // ボタン判定
    ImGui::InvisibleButton(" ", {});

    // 再生バーの作成
    ImDrawList *draw = ImGui::GetWindowDrawList();

    // 灰色バー
    // 位置、サイズ、カラー
    draw->AddRectFilled({},
                        {},
                        IM_COL32(80, 80, 80, 255));

    ImGui::SameLine();

    // min:sec を出力
    ImGui::Text(" ");

    ImGui::End();

    // ImGui描画
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}