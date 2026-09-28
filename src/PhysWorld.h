#pragma once

#include <vector>
#include <glm/glm.hpp>

class Game;
class Actor;
class BoxComponent;

class PhysWorld
{
public:
    PhysWorld(Game *game);

    void GroundCollision(Actor *actor);

    void AddBox(BoxComponent *box);
    void RemoveBox(BoxComponent *box);

private:
    Game *mGame;

    std::vector<BoxComponent *> mBoxComps;
};
