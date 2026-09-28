#pragma onces

#include <vector>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

class Renderer;
class Actor;
class Player;
class Camera;
class FollowCameraComponent;
class AssetManager;
class PhysWorld;

class Game
{
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor *actor);
    void RemoveActor(Actor *actor);

    Renderer *GetRenderer() const { return mRenderer; }
    AssetManager *GetAssetManager() const { return mAssetManager; }
    PhysWorld *GetPhysWorld() const { return mPhysWorld; }

private:
    void ProcessInput();
    void GameUpdate();
    void GenerateOutput();
    bool LoadData();
    void UnloadData();

    bool LoadModel(const std::string &filName, const std::string &modelName);
    void AttachModelComp(Actor *actor, const std::string &modelName, const std::string &animName = "");

    Renderer *mRenderer;
    AssetManager *mAssetManager;
    PhysWorld *mPhysWorld;

    std::vector<Actor *> mActor;
    Player *mPlayer;

    bool mIsRunning;
    Uint32 mTicksCount;
};