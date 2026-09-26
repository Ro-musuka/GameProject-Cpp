#pragma once

#include <glm/glm.hpp>

namespace Common
{
    const glm::vec3 UnitX{1.0f, 0.0f, 0.0f};
    const glm::vec3 UnitY{0.0f, 1.0f, 0.0f};
    const glm::vec3 UnitZ{0.0f, 0.0f, 1.0f};

    inline bool NearZero(float val, float epsilon = 0.001f)
    {
        if (fabs(val) <= epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}