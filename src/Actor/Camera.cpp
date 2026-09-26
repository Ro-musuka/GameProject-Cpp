#include "../Actor/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Game.h"

Camera::Camera(Game *game)
    : Actor(game)
{
}

void Camera::UpdateActor(float deltaTime)
{
    glm::vec3 pos = GetPosition();
    SetPosition({pos.x + mFrontSpeed * deltaTime, pos.y, pos.z + mLeftSpeed * deltaTime});
}

void Camera::ActorInput(const bool *keystate)
{
    mFrontSpeed = 0.0f;
    mLeftSpeed = 0.0f;

    if (keystate[SDL_SCANCODE_W])
    {
        mFrontSpeed += 50.0f;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        mFrontSpeed -= 50.0f;
    }

    if (keystate[SDL_SCANCODE_A])
    {
        mLeftSpeed += 50.0f;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        mLeftSpeed -= 50.0f;
    }
}