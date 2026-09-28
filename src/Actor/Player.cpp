#include "Player.h"

#include <glm/glm.hpp>

#include "../Game.h"
#include "../Common.h"

Player::Player(Game *game)
    : Actor(game),
      mFrontSpeed(.0f),
      mLeftSpeed(.0f),
      mGravitySpeed(-5.0f)
{
}

void Player::UpdateActor(float deltaTime)
{
    glm::vec3 pos = GetPosition();
    if (!Common::NearZero(mFrontSpeed))
    {
        pos += Common::UnitZ * (mFrontSpeed * deltaTime);
    }
    if (!Common::NearZero(mLeftSpeed))
    {
        pos += Common::UnitX * (mLeftSpeed * deltaTime);
    }
    pos += Common::UnitY * mGravitySpeed * deltaTime;

    SetPosition(pos);
}

void Player::ActorInput(const bool *keystate)
{
    mFrontSpeed = 0.0f;
    mLeftSpeed = 0.0f;

    if (keystate[SDL_SCANCODE_W])
    {
        mFrontSpeed += 10.0f;
    }
    if (keystate[SDL_SCANCODE_S])
    {
        mFrontSpeed -= 10.0f;
    }

    if (keystate[SDL_SCANCODE_A])
    {
        mLeftSpeed += 10.0f;
    }
    if (keystate[SDL_SCANCODE_D])
    {
        mLeftSpeed -= 10.0f;
    }
}