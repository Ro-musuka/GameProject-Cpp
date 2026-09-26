#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// 時間ごとの位置、回転、拡大
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

// それらの配列を持つ、ボーンごとのアニメーション
struct BoneAnimation
{
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
};

// 一つのアニメーションができる
struct Animation
{
    std::string name;
    float duration = 0.0f;
    // 再生速度
    float ticksPerSecond = 1.0f;

    // ボーンの名前ごとの、アニメーション
    std::unordered_map<std::string, BoneAnimation> channels;
};