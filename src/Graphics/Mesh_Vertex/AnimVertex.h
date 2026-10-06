#pragma once

#include <glm/glm.hpp>

struct AnimVertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;

    glm::ivec4 bones;
    glm::vec4 weights;
};