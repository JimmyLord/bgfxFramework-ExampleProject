//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "DataTypes.h"
#include "Game.h"
#include "Events/GameEvents.h"
#include "Objects/Player.h"
#include "Objects/PlayerController.h"
#include "Meshes/Shapes.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
{
    Init();
}

Game::~Game()
{
    for( auto& meshPair : m_pMeshes )
    {
        delete meshPair.second;
    }

    for( auto& materialPair : m_pMaterials )
    {
        delete materialPair.second;
    }

    for( auto& spriteSheetPair : m_pSpriteSheets )
    {
        delete spriteSheetPair.second;
    }

    for( auto& texturePair : m_pTextures )
    {
        delete texturePair.second;
    }

    for( auto& shaderPair : m_pShaders )
    {
        delete shaderPair.second;
    }

    for( fw::GameObject* pObject : m_Objects )
    {
        delete pObject;
    }
    
    delete m_pCamera;

    delete m_pPlayerController;

    delete m_pEventManager;

    delete m_pImGuiManager;
}

void Game::Init()
{
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, 1 );

    // General renderer settings.
    bgfx::setViewClear( 0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( 0, 0, 0, m_FWCore.GetWindowWidth(), m_FWCore.GetWindowHeight() );

    m_pEventManager = new fw::EventManager();

    // Create uniforms.
    m_pUniforms = new fw::Uniforms();

    // Create vertex formats.
    InitTextureFormats();

    // Create some meshes.
    m_pMeshes["Triangle"] = CreateTriangleMesh();
    m_pMeshes["Square"] = CreateSquareMesh();
    m_pMeshes["Sprite"] = CreateSpriteMesh();

    // Load some shaders.
    m_pShaders["SolidColor"] = new fw::ShaderProgram( "Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin" );
    m_pShaders["VertexColor"] = new fw::ShaderProgram( "Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin" );
    m_pShaders["Texture"] = new fw::ShaderProgram( "Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin" );

    // Load some textures.
    m_pTextures["Sokoban"] = new fw::Texture( "Data/Textures/Sokoban.png" );

    // Load some spritesheets.
    m_pSpriteSheets["Sokoban"] = new fw::SpriteSheet( "Data/Textures/Sokoban.json", m_pTextures["Sokoban"] );

    // UV scale and offset for the sokoban player image.
    fw::SpriteSheet::SpriteInfo info = m_pSpriteSheets["Sokoban"]->GetSpriteByName( "Player/player_01" );

    // Create some materials.
    m_pMaterials["Red"] = new fw::Material( m_pShaders["SolidColor"], nullptr, fw::color4f::Red(), false );
    m_pMaterials["Blue"] = new fw::Material( m_pShaders["SolidColor"], nullptr, fw::color4f::Blue(), false );
    m_pMaterials["Green"] = new fw::Material( m_pShaders["SolidColor"], nullptr, fw::color4f::Green(), false );
    m_pMaterials["VertexColor"] = new fw::Material( m_pShaders["VertexColor"], nullptr, fw::color4f::White(), false );
    m_pMaterials["SokobanPlayer01"] = new fw::Material( m_pShaders["Texture"], m_pTextures["Sokoban"], fw::color4f::White(), true, info.asVec4() );

    // Create a controller.
    m_pPlayerController = new PlayerController();

    // Create some GameObjects.
    m_pCamera = new fw::Camera( this, vec3(5,5,-20) );
    m_pCamera->SetLookAtPosition( vec3(5,5,0) );
    m_pPlayer = new Player( this, m_pPlayerController, "Player", vec3(6,5,-0.1f), m_pMeshes["Sprite"], m_pMaterials["SokobanPlayer01"] );
    m_Objects.push_back( m_pPlayer );

    fw::GameObject* pTestObjectToDelete = new fw::GameObject( this, "delete me", vec3(1,9,0), m_pMeshes["Square"], m_pMaterials["Blue"] );

    m_Objects.push_back( new fw::GameObject( this, "Object 1", vec3(0,0,0), m_pMeshes["Triangle"], m_pMaterials["VertexColor"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 2", vec3(10,10,0), m_pMeshes["Triangle"], m_pMaterials["Blue"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 3", vec3(5,5,0), m_pMeshes["Square"], m_pMaterials["VertexColor"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 4", vec3(1,1,0), m_pMeshes["Square"], m_pMaterials["VertexColor"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Object 5", vec3(1,9,0), m_pMeshes["Square"], m_pMaterials["Blue"] ) );

    delete pTestObjectToDelete;

    // Create an entity without a GameObject class.
    entt::entity entityID = CreateEntity();
    m_ECSRegistry.emplace<fw::TransformData>( entityID, vec3(3,7,0), vec3(0), vec3(1) );
    m_ECSRegistry.emplace<fw::NameData>( entityID, "Headless Object" );
    m_ECSRegistry.emplace<fw::MeshData>( entityID, m_pMeshes["Square"], m_pMaterials["Red"] );
}

void Game::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

    // Reset the controller.
    m_pPlayerController->StartFrame();

    // Dispatch events.
    m_pEventManager->DispatchAllEvents( deltaTime, this );
}

void Game::OnEvent(fw::Event* pEvent)
{
    // Process events.
    m_pPlayerController->OnEvent( pEvent );

    if( pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType() )
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>( pEvent );
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find( m_Objects.begin(), m_Objects.end(), pObject );
        m_Objects.erase( it );

        delete pObject;
    }

    if( pEvent->GetType() == fw::WindowResizeEvent::GetStaticEventType() )
    {
        int width = m_FWCore.GetWindowWidth();
        int height = m_FWCore.GetWindowHeight();

        m_pCamera->SetAspectRatio( (float)width/height );
    }
}

void Game::Update(float deltaTime)
{
    Editor_DisplayObjectList();

    for( fw::GameObject* pObject : m_Objects )
    {
        pObject->Update( deltaTime );
    }

    m_pCamera->Update( deltaTime );
}

void Game::Draw()
{
    //bgfx::touch( 0 );

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform( m_pUniforms->m_Map["u_Time"], &time );

    // Program the view and proj uniforms from the camera.
    m_pCamera->Enable();

    // Draw all objects.
    GameCore::Draw();

    // Show bgfx debug stats.
    ImGui::Checkbox( "Show Debug Stats", &m_ShowDebugStats );
    bgfx::dbgTextClear();
    if( m_ShowDebugStats )
    {
        bgfx::setDebug( BGFX_DEBUG_STATS );
    }
    else
    {
        bgfx::setDebug( BGFX_DEBUG_NONE );
    }

    m_pImGuiManager->EndFrame();
}

void Game::Editor_DisplayObjectList()
{
    ImGui::Begin( "Object List" );

    auto view = m_ECSRegistry.view<fw::NameData>();
    for( auto entity : view )
    {
        auto& nameData = view.get<fw::NameData>( entity );

        ImGui::Text( "%s", nameData.m_Name );
    }

    ImGui::End(); // "Object List"
}
