#include "PhysWorld.h"

#include <algorithm>

#include <SDL3/SDL.h>

#include "Collision.h"

#include "Actor/Actor.h"

#include "Component/BoxComponent.h"

PhysWorld::PhysWorld(Game *game)
    : mGame(game)
{
}

void PhysWorld::GroundCollision(Actor *actor)
{
    std::sort(mBoxComps.begin(), mBoxComps.end(),
              [](BoxComponent *a, BoxComponent *b)
              {
                  return a->GetWorldBox().min.x < b->GetWorldBox().min.x;
              });

    glm::vec3 pos = actor->GetPosition();
    for (auto &bc : mBoxComps)
    {
        if (bc->GetWorldBox().Contains(pos))
        {
            actor->SetPosition({pos.x, bc->GetWorldBox().max.y, pos.z});
        }
    }
}

void PhysWorld::AddBox(BoxComponent *box)
{
    mBoxComps.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent *box)
{
    auto iter = std::find(mBoxComps.begin(), mBoxComps.end(), box);
    if (iter == mBoxComps.end())
    {
        std::iter_swap(iter, mBoxComps.end() - 1);
        mBoxComps.pop_back();
    }
}
