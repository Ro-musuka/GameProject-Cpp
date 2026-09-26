#pragma onces
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vector>

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

    Camera *GetCamera() const { return mCamera; }

private:
    void ProcessInput();
    void GameUpdate();
    void GenerateOutput();
    bool LoadData();
    void UnloadData();

    Renderer *mRenderer;

    std::vector<Actor *> mActor;
    Camera *mCamera;

    bool mIsRunning;
    Uint32 mTicksCount;
};