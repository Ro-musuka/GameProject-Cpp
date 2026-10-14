#include "Game.h"

#include <GL/glew.h>
#include <algorithm>

#include "Actor/Actor.h"
#include "Actor/Player.h"

#include "Component/MeshComp/MeshComponent.h"
#include "Component/MeshComp/AnimMeshComponent.h"
#include "Component/FollowCameraComponent.h"
#include "Component/CollisionComp/PlayerBoxComponent.h"

#include "Collision/PhysWorld.h"
#include "Collision/Collision.h"

#include "Graphics/Animation/Skeleton.h"
#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/AnimationPlayer.h"

#include "Graphics/Renderer.h"
#include "Graphics/Mesh_Vertex/Mesh.h"
#include "Graphics/Mesh_Vertex/AnimMesh.h"

#include "Editor/Editor.h"

#include "AssetTools/AssetManager.h"
#include "AssetTools/ModelLoader.h"
#include "AssetTools/SceneSerializer.h"

Game::Game()
    : mIsRunning(true),
      mTicksCount(0.0f)
{
}

bool Game::Initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDLの初期化に失敗しました: %s", SDL_GetError());
        return false;
    }

    mPhysWorld = new PhysWorld(this);

    // rendererより、先にアセットマナージャーをメモリ確保する必要がある
    mAssetManager = new AssetManager();

    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(1024.0f, 768.0f))
    {
        SDL_Log("レンダラーの初期化に失敗しました\n");
        return false;
    }

    // エディター機能の初期化
    mEditor = new Editor(this);
    if (!mEditor->Init("#version 410"))
    {
        SDL_Log("エンジンのエディター機能の初期化に失敗しました。");
        return false;
    }

    if (!LoadData())
    {
        return false;
    }

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        GameUpdate();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    UnloadData();
    mRenderer->Shutdown();

    if (mEditor != nullptr)
    {
        delete mEditor;
    }
    delete mRenderer;
    delete mAssetManager;
    delete mPhysWorld;

    SDL_Quit();
}

void Game::ProcessInput()
{
    // イベントをポーリング
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            mIsRunning = false;
            break;
        }
        mEditor->ProcessEvent(e);
    }

    // キー入力処理
    const bool *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    for (auto actor : mActors)
    {
        actor->ProcessInput(keystate);
    }
}

void Game::GameUpdate()
{
    // 60fps
    while (SDL_GetTicks() < mTicksCount + 16.6)
    {
    }

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.5f)
    {
        deltaTime = 0.5f;
    }
    mTicksCount = SDL_GetTicks();

    // 更新中は追加actorをpendingActorsに追加する
    // 各actorを更新、衝突処理は後から行う
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mPhysWorld->CheckCollisions();
    mUpdatingActors = false;

    // 待機中だったactorを追加
    for (auto pending : mPendingActors)
    {
        pending->ComputeWorldTransform();
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // EDead状態のactorを削除
    std::vector<Actor *> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }

    // actorの削除はGameのみ行う
    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
    mEditor->Draw();
    mRenderer->SwapWindow();
}

bool Game::LoadData()
{
    // 3dモデルのロード
    // 壁・床のモデル
    if (!LoadModel("../assets/models/box.glb", "box"))
    {
        return false;
    }

    // playerのモデル
    if (!LoadModel("../assets/models/BaseMan.glb", "anim"))
    {
        return false;
    }

    // フィールドやactorのJSONデータを読み込み
    SceneSerializer sceneLoader(this);
    const char *path = "../assets/JSON/Scene.json";
    if (!sceneLoader.LoadScene(path))
    {
        SDL_Log("%sが読み込めません", path);
        return false;
    }

    mPlayer->SetAnimMeshComp(mPlayer->FindComponent<AnimMeshComponent>());

    return true;
}

bool Game::LoadModel(const std::string &fileName, const std::string &modelName)
{
    ModelLoader loader(fileName);
    if (!loader.IsValid())
    {
        SDL_Log("%sの読み込みに失敗しました", fileName.c_str());
        return false;
    }

    // 読み込みデータはアセットマネージャにセット
    if (loader.IsAnimation())
    {
        // アニメなら、メッシュ、スケルトン、アニメデータを読み込む
        mAssetManager->SetAnimMesh(modelName, loader.AnimMeshLoader());
        mAssetManager->SetSkeleton(modelName, loader.SkeletonLoader());
        mAssetManager->SetAnimations(modelName, loader.AnimationsLoader());
    }
    else
    {
        mAssetManager->SetMesh(modelName, loader.MeshLoader());
    }

    return true;
}

// 基本的に外部から呼ばれる
void Game::CreateBox()
{
    Actor *actor = new Actor(this);
    // メッシュ肉付け
    MeshComponent *mc = new MeshComponent(actor, mAssetManager->GetMesh("box"));
    mc->SetModelName("box");
    // AABB
    BoxComponent *box = new BoxComponent(actor);
    box->SetCollisionType(BoxComponent::CollisionType::EStatic);
    box->SetObjectBox(Collision::AABB(glm::vec3(1.0f), glm::vec3(-1.0f)));
}

void Game::UnloadData()
{
    while (!mActors.empty())
    {
        delete mActors.back();
    }
    mAssetManager->Unload();
}

void Game::AddActor(Actor *actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor *actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}