#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Game;
class Actor;
class Component;
class BoxComponent;

class SceneSerializer
{
public:
    SceneSerializer(Game *game);
    bool SaveScene_JSON(const std::string &path);
    bool LoadModels_JSON(const std::string &path);
    bool LoadScene_JSON(const std::string &path);

private:
    bool SaveComponents(nlohmann::json &compJSON, Component *comp);

    bool LoadModelAsset(const std::string &path, const std::string &modelName);

    Actor *CreateActor(const nlohmann::json &compJSON);
    bool AttachComp(Actor *actor, const nlohmann::json &compJSON);
    bool AttachModelComp(Actor *actor, const std::string &modelName);
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