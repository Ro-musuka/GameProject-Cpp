#pragma once

#include <string>
#include <glm/glm.hpp>

struct Bone
{
    uint32_t ID;
    std::string name;

    glm::mat4 offset{1.0f};
    glm::mat4 bindLocalTransform{1.0f};
    glm::mat4 localTransform{1.0f};
    glm::mat4 globalTransform{1.0f};

    // 親ボーンID
    int parentID = -1;
    // 子ボーンID
    std::vector<uint32_t> childrenIDs;
};