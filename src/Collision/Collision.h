#pragma once

#include <glm/glm.hpp>

namespace Collision
{

    struct AABB
    {
        AABB(const glm::vec3 &max, const glm::vec3 &min);
        void UpdateMinMax(const glm::vec3 &point);
        void Rotate(const glm::quat &q);

        bool Contains(const glm::vec3 &point) const;

        glm::vec3 min;
        glm::vec3 max;
    };

    bool Intersect(const AABB &a, const AABB &b);
};