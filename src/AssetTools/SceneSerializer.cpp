#include "SceneSerializer.h"

#include <fstream>

#include <SDL3/SDL.h>

#include "Core/Game.h"

#include "Actor/Actor.h"
#include "Actor/Player.h"

#include "Component/FollowCameraComponent.h"
#include "Component/CollisionComp/BoxComponent.h"
#include "Component/CollisionComp/PlayerBoxComponent.h"
#include "Component/MeshComp/MeshComponent.h"
#include "Component/MeshComp/AnimMeshComponent.h"

#include "Collision/Collision.h"

#include "Graphics/Mesh_Vertex/Mesh.h"
#include "Graphics/Mesh_Vertex/AnimMesh.h"

#include "AssetTools/AssetManager.h"
#include "AssetTools/ModelLoader.h"

SceneSerializer::SceneSerializer(Game *game)
    : mGame(game)
{
}

bool SceneSerializer::SaveScene_JSON(const std::string &path)
{
    // 保存ファイル
    std::ofstream file(path);
    if (!file.is_open())
    {
        SDL_Log("%sが読み込めません", path.c_str());
        return false;
    }

    // 保存するJSONデータ
    nlohmann::json root;
    // actor配列データを作成
    root["actors"] = nlohmann::json::array();

    for (Actor *actor : mGame->GetActors())
    {
        nlohmann::json actorJSON;

        // Actorの型名を保存
        actorJSON["type"] = actor->GetTypeName();

        // 各データをJSONのオブジェクトにして、componentJSON配列に追加していく
        for (Component *comp : actor->GetComponents())
        {
            nlohmann::json compJSON = nlohmann::json::object();
            SaveComponents_JSON(compJSON, comp);
            actorJSON["components"].push_back(compJSON);
        }

        // transformデータを保存
        nlohmann::json transform = nlohmann::json::object();
        transform["position"] = ToJSON(actor->GetPosition());
        transform["rotation"] = ToJSON(actor->GetRotation());
        transform["scale"] = ToJSON(actor->GetScale());
        actorJSON["transform"] = transform;

        // actorデータを配列に追加
        root["actors"].push_back(actorJSON);
    }

    // JSONデータをファイルに書き込む
    file << root.dump(4);

    return file.good();
}

bool SceneSerializer::LoadModels_JSON(const std::string &path)
{
    // JSONファイル読み込み
    std::ifstream file(path);
    if (!file.is_open())
    {
        SDL_Log("%sが読み込めません", path.c_str());
        return false;
    }
    nlohmann::json JSON_Data;
    file >> JSON_Data;

    for (const nlohmann::json &modelJSON : JSON_Data.at("models"))
    {
        // 3Dモデル読み込み
        if (!LoadModelAsset(modelJSON.at("path"), modelJSON.at("name")))
        {
            return false;
        }
    }

    return true;
}

bool SceneSerializer::LoadScene_JSON(const std::string &path)
{
    // JSONファイル読み込み
    std::ifstream file(path);
    if (!file.is_open())
    {
        SDL_Log("%sが読み込めません", path.c_str());
        return false;
    }
    nlohmann::json JSON_Data;
    file >> JSON_Data;

    for (const nlohmann::json &actorJSON : JSON_Data.at("actors"))
    {
        Actor *actor = CreateActor(actorJSON);
        if (actor == nullptr)
        {
            return false;
        }

        for (const nlohmann::json &compJSON : actorJSON.at("components"))
        {
            if (!AttachComp(actor, compJSON))
            {
                return false;
            }
        }

        const nlohmann::json &transform = actorJSON.at("transform");
        actor->SetPosition(ToVec3(transform.at("position")));
        actor->SetRotation(ToQuat(transform.at("rotation")));
        actor->SetScale(ToVec3(transform.at("scale")));

        // 初期化
        actor->Initialize();
    }

    return true;
}

bool SceneSerializer::SaveComponents_JSON(nlohmann::json &compJSON, Component *comp)
{
    // それぞれのcompの型をキャストできるかで確かめる
    // 派生クラスは元クラスでキャストできるので注意

    // mesh == nullptr
    if (MeshComponent *mesh = dynamic_cast<MeshComponent *>(comp))
    {
        compJSON["type"] = mesh->GetTypeName();
        compJSON["model"] = mesh->GetModelName();
    }
    else if (AnimMeshComponent *amesh = dynamic_cast<AnimMeshComponent *>(comp))
    {
        compJSON["type"] = amesh->GetTypeName();
        compJSON["model"] = amesh->GetModelName();
    }
    // (PlayerBoxComponent *)は,(BoxComponent *)にキャストされるが、データが同じなので今回は良い
    // BoxComponentとは異なる変数などを設定する際は別定義が必要
    if (BoxComponent *box = dynamic_cast<BoxComponent *>(comp))
    {
        compJSON["type"] = box->GetTypeName();

        // 衝突タイプ
        switch (box->GetCollisionType())
        {
        case BoxComponent::CollisionType::EStatic:
            compJSON["collisionType"] = "Static";
            break;
        case BoxComponent::CollisionType::EDynamic:
            compJSON["collisionType"] = "Dynamic";
            break;
        case BoxComponent::CollisionType::ETrigger:
            compJSON["collisionType"] = "Trigger";
            break;
        default:
            compJSON["collisionType"] = " ";
        }

        // AABB
        Collision::AABB aabb = box->GetObjectBox();
        compJSON["max"] = ToJSON(aabb.max);
        compJSON["min"] = ToJSON(aabb.min);
    }
    // else if (dynamic_cast<FollowCameraComponent *>(comp) != nullptr)
    else
    {
        compJSON["type"] = comp->GetTypeName();
    }

    return true;
}

bool SceneSerializer::LoadModelAsset(const std::string &path, const std::string &modelName)
{
    ModelLoader loader(path);
    if (!loader.IsValid())
    {
        SDL_Log("%sの読み込みに失敗しました", path.c_str());
        return false;
    }

    // モデルデータはアセットマネージャで管理する
    AssetManager *assetManager = mGame->GetAssetManager();
    if (loader.IsAnimation())
    {
        // アニメなら、メッシュ、スケルトン、アニメデータを読み込む
        assetManager->SetAnimMesh(modelName, loader.AnimMeshLoader());
        assetManager->SetSkeleton(modelName, loader.SkeletonLoader());
        assetManager->SetAnimations(modelName, loader.AnimationsLoader());
    }
    else
    {
        assetManager->SetMesh(modelName, loader.MeshLoader());
    }

    return true;
}

Actor *SceneSerializer::CreateActor(const nlohmann::json &actorJSON)
{
    Actor *actor = nullptr;
    std::string type = actorJSON.at("type").get<std::string>();
    if (type == "Actor")
    {
        actor = new Actor(mGame);
    }
    else if (type == "Player")
    {
        actor = new Player(mGame);
    }
    else
    {
        SDL_Log("対応するActorがありませんでした。: %s", type.c_str());
    }

    return actor;
}

bool SceneSerializer::AttachComp(Actor *actor, const nlohmann::json &compJSON)
{
    std::string componentType = compJSON.at("type").get<std::string>();
    if (componentType == "BoxComponent")
    {
        BoxComponent *box = new BoxComponent(actor);
        if (!SettingBoxComp(box, compJSON))
        {
            return false;
        }
    }
    else if (componentType == "PlayerBoxComponent")
    {
        BoxComponent *playerBox = new PlayerBoxComponent(actor);
        if (!SettingBoxComp(playerBox, compJSON))
        {
            return false;
        }
    }
    else if (componentType == "MeshComponent" || componentType == "AnimMeshComponent")
    {
        if (!AttachModelComp(actor, compJSON.at("model").get<std::string>()))
        {
            return false;
        }
    }
    else if (componentType == "FollowCameraComponent")
    {
        new FollowCameraComponent(actor);
    }
    else
    {
        SDL_Log("対応するComponentがありませんでした。: %s", componentType.c_str());
        return false;
    }

    return true;
}

bool SceneSerializer::AttachModelComp(Actor *actor, const std::string &modelName)
{
    // モデル名、アセットマネージャを取得
    AssetManager *assets = mGame->GetAssetManager();

    // 名前が登録されているか確認
    Mesh *mesh = assets->GetMesh(modelName);
    if (mesh != nullptr)
    {
        MeshComponent *mc = new MeshComponent(actor, mesh);
        mc->SetModelName(modelName);
        return true;
    }

    AnimMesh *amesh = assets->GetAnimMesh(modelName);
    if (amesh != nullptr)
    {
        // 骨格とアニメデータをまとめて、comp作成
        AnimationPlayer player(assets->GetSkeleton(modelName), *assets->GetAnimations(modelName));
        AnimMeshComponent *amc = new AnimMeshComponent(actor, amesh, player);
        amc->SetModelName(modelName);
        return true;
    }

    SDL_Log("%s という名前の3Dモデルは登録されていません。", modelName.c_str());
    return false;
}

bool SceneSerializer::SettingBoxComp(BoxComponent *box, const nlohmann::json &compJSON)
{
    // 衝突物のタイプの設定
    BoxComponent::CollisionType colType;
    const std::string &collisionType = compJSON.at("collisionType").get<std::string>();
    if (collisionType == "Static")
    {
        colType = BoxComponent::CollisionType::EStatic;
    }
    else if (collisionType == "Dynamic")
    {
        colType = BoxComponent::CollisionType::EDynamic;
    }
    else if (collisionType == "Trigger")
    {
        colType = BoxComponent::CollisionType::ETrigger;
    }
    else
    {
        SDL_Log("対応するCollisionTypeがありませんでした。: %s", collisionType.c_str());
        return false;
    }
    box->SetCollisionType(colType);

    // AABBの設定
    glm::vec3 max = ToVec3(compJSON.at("max"));
    glm::vec3 min = ToVec3(compJSON.at("min"));
    box->SetObjectBox(Collision::AABB(max, min));

    return true;
}