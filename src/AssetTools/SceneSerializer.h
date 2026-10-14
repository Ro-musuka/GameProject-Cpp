#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Game;
class Actor;
class Component;
class BoxComponent;

// JSONデータの読み書きを行うクラス
class SceneSerializer
{
public:
    SceneSerializer(Game *game);
    // actorやcompのデータを書き込む
    bool SaveScene_JSON(const std::string &path);
    // 3Dモデルの読み込み
    bool LoadModels_JSON(const std::string &path);
    // actorやcompの読み込み
    bool LoadScene_JSON(const std::string &path);

private:
    // コンポーネントのデータ書き込み
    bool SaveComponents_JSON(nlohmann::json &compJSON, Component *comp);

    // 3Dモデルのデータ読み込み
    bool LoadModelAsset(const std::string &path, const std::string &modelName);

    // actor作成
    Actor *CreateActor(const nlohmann::json &compJSON);
    // コンポーネントの肉付け
    bool AttachComp(Actor *actor, const nlohmann::json &compJSON);
    // 3Dモデルの肉付け
    bool AttachModelComp(Actor *actor, const std::string &modelName);
    // BoxCompにタイプやAABBを設定する
    bool SettingBoxComp(BoxComponent *box, const nlohmann::json &compJSON);

    nlohmann::json ToJSON(const glm::vec3 &v) { return nlohmann::json::array({v.x, v.y, v.z}); }
    nlohmann::json ToJSON(const glm::quat &q) { return nlohmann::json::array({q.w, q.x, q.y, q.z}); }

    glm::vec3 ToVec3(const nlohmann::json &j)
    {
        return glm::vec3(j.at(0).get<float>(),
                         j.at(1).get<float>(),
                         j.at(2).get<float>());
    }
    glm::quat ToQuat(const nlohmann::json &j)
    {
        return glm::quat(j.at(0).get<float>(),
                         j.at(1).get<float>(),
                         j.at(2).get<float>(),
                         j.at(3).get<float>());
    }

    Game *mGame;
};