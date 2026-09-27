#pragma onces

#include <vector>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

class Renderer;
class Actor;
class Camera;

class Game
{
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor *actor);
    void RemoveActor(Actor *actor);

private:
    void ProcessInput();
    void GameUpdate();
    void GenerateOutput();
    bool LoadData();
    void UnloadData();

    Renderer *mRenderer;

    std::vector<Actor *> mActor;

    bool mIsRunning;
    Uint32 mTicksCount;
};