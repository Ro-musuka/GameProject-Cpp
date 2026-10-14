#pragma onces

#include <vector>

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

class Renderer;
class Editor;
class AssetManager;
class PhysWorld;
class Actor;
class Player;

class Game
{
public:
    friend class SceneSerializer;

    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor *actor);
    void RemoveActor(Actor *actor);

    // box作成
    void CreateBox();

    // ゲッター/セッター
    Renderer *GetRenderer() const { return mRenderer; }
    AssetManager *GetAssetManager() const { return mAssetManager; }
    PhysWorld *GetPhysWorld() const { return mPhysWorld; }
    void SetPlayer(Player *player) { mPlayer = player; }

private:
    void ProcessInput();
    void GameUpdate();
    void GenerateOutput();
    bool LoadData();
    void UnloadData();

    // SceneSerializer用の関数
    const std::vector<Actor *> &GetActors() const { return mActors; }

    std::vector<Actor *> mActors;
    // 待機中のactors
    std::vector<Actor *> mPendingActors;
    // actorの更新中を知らせる
    bool mUpdatingActors;
    Player *mPlayer;

    Renderer *mRenderer;
    Editor *mEditor;
    AssetManager *mAssetManager;
    PhysWorld *mPhysWorld;

    bool mIsRunning;
    Uint32 mTicksCount;
};