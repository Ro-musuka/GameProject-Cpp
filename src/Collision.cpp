#include "Collision.h"

#include <array>
#include <algorithm>

#include <glm/gtc/quaternion.hpp>

Collision::AABB::AABB(const glm::vec3 &max, const glm::vec3 &min)
    : max(max),
      min(min)
{
}

void Collision::AABB::UpdateMinMax(const glm::vec3 &point)
{
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);

    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);
}

void Collision::AABB::Rotate(const glm::quat &q)
{
    std::array<glm::vec3, 8> points;
    points[0] = min;
    points[1] = {max.x, min.y, min.z};
    points[2] = {min.x, max.y, min.z};
    points[3] = {min.x, min.y, max.z};

    points[4] = {min.x, max.y, max.z};
    points[5] = {max.x, min.y, max.z};
    points[6] = {max.x, max.y, min.z};
    points[7] = max;

    glm::vec3 p = points[0] * q;
    min = p;
    max = p;
    for (int i = 1; i < points.size(); i++)
    {
        p = points[i] * q;
        UpdateMinMax(p);
    }
}

bool Collision::Intersect(const AABB &a, const AABB &b)
{
    bool no = a.max.x < b.min.x || a.max.y < b.min.y || a.max.z < b.min.z ||
              b.max.x < a.min.x || b.max.y < a.min.y || b.max.z < a.min.z;

    return !no;
}