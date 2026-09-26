#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct KeyPosition
{
    float time;
    glm::vec3 value;
};

struct KeyRotation
{
    float time;
    glm::quat value;
};

struct KeyScale
{
    float time;
    glm::vec3 value;
};

struct BoneAnimation
{
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
};

struct Animation
{
    std::string name;
    float duration = 0.0f;
    float ticksPerSecond = 1.0f;

    std::unordered_map<std::string, BoneAnimation> channels;
};