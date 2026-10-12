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

    void CheckCollisions();

    void AddBox(BoxComponent *box);
    void RemoveBox(BoxComponent *box);

    const std::vector<BoxComponent *> &GetBoxComps() const { return mBoxComps; }

private:
    Game *mGame;

    std::vector<BoxComponent *> mBoxComps;
};
