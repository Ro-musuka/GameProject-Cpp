#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Common
{
    const glm::vec3 UnitX(1.0f, 0.0f, 0.0f);
    const glm::vec3 UnitY(0.0f, 1.0f, 0.0f);
    const glm::vec3 UnitZ(0.0f, 0.0f, 1.0f);
    const glm::vec3 ZeroVec3(0.0f, 0.0f, 0.0f);
    const glm::quat UnitQuat = glm::identity<glm::quat>();
    const glm::mat4 ZUp_To_YUp = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), Common::UnitX);

    inline bool NearZero(float val, float epsilon = 0.001f)
    {
        if (val * val <= epsilon * epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    inline bool NearZero(const glm::vec3 &vec, float epsilon = 0.001f)
    {
        if ((vec.x * vec.x + vec.y * vec.y + vec.z * vec.z) <= epsilon * epsilon)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}