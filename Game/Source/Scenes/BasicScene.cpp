//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"
#include "BasicScene.h"
#include "Game.h"
#include "Objects/Player.h"

BasicScene::BasicScene(Game* pGame)
    : Scene( pGame )
{
    PlayerController* pController = pGame->GetController();
    fw::ResourceManager* pResources = pGame->GetResourceManager();
    
    // Create some GameObjects.
    m_pCamera = new fw::Camera( this, vec3(5,5,-20) );
    m_pCamera->SetLookAtPosition( vec3(5,5,0) );
    //m_Objects.push_back( m_pCamera );
    m_pPlayer = new Player( this, pController, "Player", vec3(6,5,-0.1f), pResources->GetMesh("Sprite"), pResources->GetMaterial("SokobanPlayer01") );
    m_Objects.push_back( m_pPlayer );

    fw::GameObject* pTestObjectToDelete = new fw::GameObject( this, "delete me", vec3(1,9,0), pResources->GetMesh("Square"), pResources->GetMaterial("Blue") );

    m_Objects.push_back( new fw::GameObject( this, "Object 1", vec3(0,0,0), pResources->GetMesh("Triangle"), pResources->GetMaterial("VertexColor") ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 2", vec3(10,10,0), pResources->GetMesh("Triangle"), pResources->GetMaterial("Blue") ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 3", vec3(5,5,0), pResources->GetMesh("Square"), pResources->GetMaterial("VertexColor") ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 4", vec3(1,1,0), pResources->GetMesh("Square"), pResources->GetMaterial("VertexColor") ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 5", vec3(1,9,0), pResources->GetMesh("Square"), pResources->GetMaterial("Blue") ) );
    
    m_Objects.push_back( new fw::GameObject( this, "Dissolving Object", vec3(-2,0,0), pResources->GetMesh("Sprite"), pResources->GetMaterial("DissolvingSokobanPlayer") ) );

    delete pTestObjectToDelete;

    // Create a base GameObject with no components.
    m_Objects.push_back( new fw::GameObject( this ) );

    //// Create an entity without a GameObject class.
    //flecs::entity entityID = CreateEntity();
    //entityID.set<fw::TransformData>( {vec3(3,7,0), vec3(0), vec3(1)} );
    //entityID.set<fw::NameData>( {"Headless Object"} );
    //entityID.set<fw::MeshData>( {pResources->GetMesh("Square"), pResources->GetMaterial("Red")} );
}

BasicScene::~BasicScene()
{
    delete m_pCamera;
}

void BasicScene::CreateComponentManager()
{
    m_pComponentManager = new fw::ComponentManager();
}

void BasicScene::Init()
{
    Scene::Init();
}

void BasicScene::OnShutdown()
{
    Scene::OnShutdown();
}

void BasicScene::StartFrame(float deltaTime)
{
    Scene::StartFrame( deltaTime );
}

void BasicScene::OnEvent(fw::Event* pEvent)
{
    Scene::OnEvent( pEvent );
    
    if( pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType() )
    {
        int width = m_pGameCore->GetFramework()->GetWindowClientWidth();
        int height = m_pGameCore->GetFramework()->GetWindowClientHeight();

        m_pCamera->SetAspectRatio( (float)width/height );
    }
}

void BasicScene::Update(float deltaTime)
{
    Scene::Update( deltaTime );

    m_pCamera->Update( deltaTime );
}

void BasicScene::Draw(int viewID)
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

Game* BasicScene::GetGame()
{
    return static_cast<Game*>( m_pGameCore );
}
