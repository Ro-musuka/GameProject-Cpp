#include "Editor.h"

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
    // IMGUIのバージョンチェック
    IMGUI_CHECKVERSION();
    // Context作成
    ImGui::CreateContext();
    // ダークスタイルに設定
    ImGui::StyleColorsDark();

    // SDL3_OpenGL3の初期化
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
    // 新しい描画フレームの作成
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    // ImGuiのウィンドウ設定
    ImGui::Begin("Box Component Editor");

    auto boxes = mGame->GetPhysWorld()->GetBoxComps();
    // 一覧表示
    if (ImGui::BeginListBox(" ", ImVec2(-FLT_MIN, 180.0f)))
    {
        for (size_t i = 0; i < boxes.size(); i++)
        {
            BoxComponent *box = boxes[i];
            bool selected = (mSelectedBox == box);
            // 押された場合に実行
            if (ImGui::Selectable(std::to_string(i).c_str(), selected))
            {
                mSelectedBox = box;
                // 回転は四元数をvec3に戻す処理が重いので、今計算する
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
    // 同一行に描画
    ImGui::SameLine();
    // 削除
    if (ImGui::Button("Remove Box") && mSelectedBox != nullptr)
    {
        mSelectedBox->GetOwner()->SetState(Actor::EDead);
        mSelectedBox = nullptr;
    }

    // 選択中のBoxを編集
    if (mSelectedBox != nullptr)
    {
        // 区切り線
        ImGui::Separator();

        Actor *actor = mSelectedBox->GetOwner();
        // それぞれ押された場合のみ実行する
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
    // ウィンドウ設定終了
    ImGui::End();

    // ImGui描画
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}