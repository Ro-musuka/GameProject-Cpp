#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

class Game;
class Actor;
class Component;

class SceneSerializer
{
public:
    SceneSerializer(Game *game);
    bool SaveScene(const std::string &fileName);
    bool LoadScene(const std::string &fileName);

private:
    Actor *CreateActor(const nlohmann::json &compJSON);
    bool AttachComp(Actor *actor, const nlohmann::json &compJSON);
    bool AttachModelComp(Actor *actor, const std::string &modelName);

    glm::vec3 ToVec3(const nlohmann::json &j);
    glm::quat ToQuat(const nlohmann::json &j);

    Game *mGame;
};