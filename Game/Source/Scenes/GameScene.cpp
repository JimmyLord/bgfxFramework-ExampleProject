//
// Copyright (c) 2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"
#include "GameScene.h"
#include "Game.h"
#include "GameDataTypes.h"
#include "GameComponents/GameSystems.h"

GameScene::GameScene(Game* pGame)
    : Scene( pGame )
{
    m_pCamera = new fw::Camera( this, vec3(5,5,-20) );
    m_pCamera->SetLookAtPosition( vec3(5,5,0) );
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
    Scene::Init();
}

void GameScene::OnShutdown()
{
    Scene::OnShutdown();
}

void GameScene::StartFrame(float deltaTime)
{
    Scene::StartFrame( deltaTime );
}

void GameScene::OnEvent(fw::Event* pEvent)
{
    Scene::OnEvent( pEvent );
    
    if( pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType() )
    {
        int width = m_pGameCore->GetFramework()->GetWindowClientWidth();
        int height = m_pGameCore->GetFramework()->GetWindowClientHeight();

        m_pCamera->SetAspectRatio( (float)width/height );
    }
}

void GameScene::Update(float deltaTime)
{
    Scene::Update( deltaTime );

    UpdateAllPlayerComponents( m_pComponentManager, deltaTime );

    m_pCamera->Update( deltaTime );
}

void GameScene::Draw(int viewID)
{
    fw::ResourceManager* pResources = m_pGameCore->GetResourceManager();

    if( viewID == fw::EditorViews::EditorView_Game )
    {
        ivec2 size = GetGame()->GetGameWindowSize();
        m_pCamera->SetAspectRatio( (float)size.x / size.y );
        m_pCamera->Update( 0 );

        // Program the view and proj uniforms from the camera.
        m_pCamera->Enable( viewID );
    }
    else
    {
        //ivec2 size = GetGame()->GetEditorWindowSize();
        //m_pCamera->SetAspectRatio( (float)size.x / size.y );
        //m_pCamera->Update( 0 );
    }

    // Draw all objects.
    Scene::Draw( viewID );
}

Game* GameScene::GetGame()
{
    return static_cast<Game*>( m_pGameCore );
}
