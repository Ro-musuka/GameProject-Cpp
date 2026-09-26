#include "Game.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

#include "Actor/Actor.h"
#include "Actor/Camera.h"

#include "Component/MeshComponent.h"

#include "Graphics/Shader.h"

#include "Renderer.h"

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

    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(1024.0f, 768.0f))
    {
        delete mRenderer;
        mRenderer = nullptr;

        SDL_Log("レンダラーの初期化に失敗しました\n");
        return false;
    }

    LoadData();

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
    }

    // キー入力処理
    const bool *keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    for (auto actor : mActor)
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

    for (auto actor : mActor)
    {
        actor->Update(deltaTime);
    }
}

void Game::GenerateOutput()
{
    mRenderer->Draw();
}

bool Game::LoadData()
{
    mCamera = new Camera(this);
    return true;
}

void Game::UnloadData()
{
    mRenderer->UnloadData();
}

void Game::AddActor(Actor *actor)
{
    mActor.emplace_back(actor);
}

void Game::RemoveActor(Actor *actor)
{
    auto iter = std::find(mActor.begin(), mActor.end(), actor);
    if (iter != mActor.end())
    {
        std::iter_swap(iter, mActor.end() - 1);
        mActor.pop_back();
    }
}