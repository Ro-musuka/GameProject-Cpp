#pragma onces

#include <vector>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include "Component/CollisionComp/BoxComponent.h"

class Renderer;
class Editor;
class AssetManager;
class PhysWorld;
class Actor;
class Player;
class Camera;
class FollowCameraComponent;
class BoxComponent;

class Game
{
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor *actor);
    void RemoveActor(Actor *actor);

    void CreateBox();

    // ゲッター/セッター
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
    // 3Dモデルの肉付け
    void AttachModelComp(Actor *actor, const std::string &modelName, const std::string &animName = "");
    // AABBの肉付け
    void AttachBoxComp(Actor *actor, BoxComponent::CollisionType type,
                       const glm::vec3 &max_AABB, const glm::vec3 &min_AABB);

    Renderer *mRenderer;
    Editor *mEditor;
    AssetManager *mAssetManager;
    PhysWorld *mPhysWorld;

    bool mUpdatingActors;
    std::vector<Actor *> mPendingActors;
    std::vector<Actor *> mActors;
    Player *mPlayer;

    bool mIsRunning;
    Uint32 mTicksCount;
};