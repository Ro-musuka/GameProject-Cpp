#include "Editor.h"

#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "external/imgui/imgui_impl_sdl3.h"

#include "Core/Game.h"
#include "Core/Common.h"
#include "Collision/PhysWorld.h"
#include "Graphics/Renderer.h"
#include "Actor/Actor.h"
#include "Component/CollisionComp/BoxComponent.h"

Editor::Editor(Game *game)
    : mGame(game),
      mSelectedBox(nullptr),
      mAngleXYZ(0.0f)
{
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

    Renderer *renderer = mGame->GetRenderer();

    if (!ImGui_ImplSDL3_InitForOpenGL(renderer->GetWindow(), renderer->GetContext()))
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

    ImGui::Begin("Box Component Editor");

    auto boxes = mGame->GetPhysWorld()->GetBoxComps();

    // 一覧
    if (ImGui::BeginListBox("Boxes", ImVec2(-FLT_MIN, 180.0f)))
    {
        for (size_t i = 0; i < boxes.size(); i++)
        {
            BoxComponent *box = boxes[i];

            // Boxごとに識別できる名前を使用する
            bool selected = (mSelectedBox == box);

            if (ImGui::Selectable(std::to_string(i).c_str(), selected))
            {
                mSelectedBox = box;

                glm::quat rotation = box->GetOwner()->GetRotation();
                mAngleXYZ = glm::degrees(glm::eulerAngles(rotation));
            }
        }

        ImGui::EndListBox();
    }

    // Boxの追加
    if (ImGui::Button("Add Box"))
    {
        mGame->CreateBox();
        mSelectedBox = mGame->GetPhysWorld()->GetBoxComps().back();
    }

    // 選択中のBoxを編集
    if (mSelectedBox != nullptr)
    {
        ImGui::Separator();

        Actor *actor = mSelectedBox->GetOwner();

        // 位置
        glm::vec3 pos = actor->GetPosition();
        if (ImGui::InputFloat3("Position: X, Y, Z", &pos.x))
        {
            actor->SetPosition(pos);
        }

        // 回転
        if (ImGui::SliderFloat3("Roll Pitch Yaw", &mAngleXYZ.x, 0.0f, 359.9f))
        {
            glm::vec3 radians = glm::radians(mAngleXYZ);

            glm::quat q = glm::identity<glm::quat>();
            q *= glm::angleAxis(radians.x, Common::UnitX);
            q *= glm::angleAxis(radians.y, Common::UnitY);
            q *= glm::angleAxis(radians.z, Common::UnitZ);

            actor->SetRotation(q);
        }

        // 拡大
        glm::vec3 scale = actor->GetScale();
        if (ImGui::InputFloat3("Scale: X, Y, Z", &scale.x))
        {
            actor->SetScale(scale);
        }
    }

    ImGui::End();

    // ImGui描画
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}