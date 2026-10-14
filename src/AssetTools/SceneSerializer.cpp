#include "SceneSerializer.h"

#include <fstream>

#include <SDL3/SDL.h>
#include <nlohmann/json.hpp>
#include <glm/gtc/quaternion.hpp>

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

SceneSerializer::SceneSerializer(Game *game)
    : mGame(game)
{
}

bool SceneSerializer::SaveScene(const std::string &fileName)
{
    return false;
}

bool SceneSerializer::LoadScene(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
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
    Component *comp = nullptr;
    std::string componentType = compJSON.at("type").get<std::string>();
    if (componentType == "BoxComponent")
    {
        BoxComponent *box = new BoxComponent(actor);

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
        Collision::AABB aabb(ToVec3(compJSON.at("max")), ToVec3(compJSON.at("min")));
        box->SetObjectBox(aabb);
    }
    else if (componentType == "PlayerBoxComponent")
    {
        PlayerBoxComponent *playerBox = new PlayerBoxComponent(actor);

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
        playerBox->SetCollisionType(colType);

        Collision::AABB aabb(ToVec3(compJSON.at("max")), ToVec3(compJSON.at("min")));
        playerBox->SetObjectBox(aabb);
    }
    else if (componentType == "MeshComponent")
    {
        // モデル名、アセットマネージャを取得
        const std::string &modelName = compJSON.at("model").get<std::string>();
        AssetManager *assets = mGame->GetAssetManager();

        // 名前が登録されているか確認
        Mesh *mesh = assets->GetMesh(modelName);
        if (mesh == nullptr)
        {
            SDL_Log("%s という名前の3Dモデルは登録されていません。", modelName.c_str());
            return false;
        }

        MeshComponent *mc = new MeshComponent(actor, mesh);
    }
    else if (componentType == "AnimMeshComponent")
    {
        const std::string &modelName = compJSON.at("model").get<std::string>();
        AssetManager *assets = mGame->GetAssetManager();

        AnimMesh *amesh = assets->GetAnimMesh(modelName);
        if (amesh == nullptr)
        {
            SDL_Log("%s という名前の3Dモデルは登録されていません。", modelName.c_str());
            return false;
        }

        // 骨格とアニメデータをまとめて、comp作成
        AnimationPlayer animPlayer(assets->GetSkeleton(modelName), *assets->GetAnimations(modelName));
        AnimMeshComponent *amc = new AnimMeshComponent(actor, amesh, animPlayer);
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

glm::vec3 SceneSerializer::ToVec3(const nlohmann::json &j)
{
    return glm::vec3(j.at(0).get<float>(),
                     j.at(1).get<float>(),
                     j.at(2).get<float>());
}

glm::quat SceneSerializer::ToQuat(const nlohmann::json &j)
{
    return glm::quat(j.at(0).get<float>(),
                     j.at(1).get<float>(),
                     j.at(2).get<float>(),
                     j.at(3).get<float>());
}