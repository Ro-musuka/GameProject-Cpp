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

void PhysWorld::CheckCollisions()
{
    // ソートしておくことで、試行回数を減らす
    std::sort(mBoxComps.begin(), mBoxComps.end(),
              [](BoxComponent *a, BoxComponent *b)
              {
                  return a->GetWorldBox().min.x < b->GetWorldBox().min.x;
              });

    for (size_t i = 0; i < mBoxComps.size(); ++i)
    {
        BoxComponent *a = mBoxComps[i];
        float max = a->GetWorldBox().max.x;
        for (size_t j = i + 1; j < mBoxComps.size(); ++j)
        {
            BoxComponent *b = mBoxComps[j];
            // これ以降は衝突がないので抜ける
            if (b->GetWorldBox().min.x > max)
            {
                break;
            }

            if (Collision::Intersect(a->GetWorldBox(), b->GetWorldBox()))
            {
                // 衝突処理
                a->OnCollision(b);
                b->OnCollision(a);
            }
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
