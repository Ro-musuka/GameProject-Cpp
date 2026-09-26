#pragma once

#include <glm/glm.hpp>
#include <cstdlib>

#include "../Actor/Actor.h"

class Camera : public Actor
{
public:
    Camera(class Game *game);

    void UpdateActor(float deltaTime) override;

    void ActorInput(const bool *keystate) override;

private:
    const glm::vec3 &UpdateDirection();

    float mFrontSpeed;
    float mLeftSpeed;
};