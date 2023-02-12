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
#include "Meshes/Shapes.h"
#include "Meshes/VertexFormats.h"
#include "Objects/PlayerController.h"
#include "Scenes/BasicScene.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
{
    Init();
}

Game::~Game()
{
    delete m_pPlayerController;
}

void Game::Init()
{
    // General renderer settings.
    bgfx::setViewClear( 0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( 0, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight() );

    m_pEventManager = new fw::EventManager();

    // Create uniforms.
    m_pUniforms = new fw::Uniforms();

    // Create vertex formats.
    InitVertexFormats();

    m_pResources = new fw::ResourceManager();

    // Create some meshes.
    m_pResources->AddMesh( CreateTriangleMesh("Triangle") );
    m_pResources->AddMesh( CreateSquareMesh("Square") );
    m_pResources->AddMesh( CreateSpriteMesh("Sprite") );

    // Load some shaders.
    m_pResources->AddShader( new fw::ShaderProgram( "SolidColor", "Data/Shaders/", "SolidColor.vert.bin", "SolidColor.frag.bin" ) );
    m_pResources->AddShader( new fw::ShaderProgram( "VertexColor", "Data/Shaders/", "VertexColor.vert.bin", "VertexColor.frag.bin" ) );
    m_pResources->AddShader( new fw::ShaderProgram( "Texture", "Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin" ) );

    // Load some textures.
    m_pResources->AddTexture( new fw::Texture( "Sokoban", "Data/Textures/Sokoban.png" ) );

    // Load some spritesheets.
    m_pResources->AddSpriteSheet( new fw::SpriteSheet( "Sokoban", "Data/Textures/Sokoban.json", m_pResources->GetTexture("Sokoban") ) );

    // UV scale and offset for the sokoban player image.
    fw::SpriteSheet::SpriteInfo info = m_pResources->GetSpriteSheet("Sokoban")->GetSpriteByName( "Player/player_01" );

    // Create some materials.
    m_pResources->AddMaterial( new fw::Material( "Red", m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Red(), false ) );
    m_pResources->AddMaterial( new fw::Material( "Blue", m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Blue(), false ) );
    m_pResources->AddMaterial( new fw::Material( "Green", m_pResources->GetShader("SolidColor"), nullptr, fw::color4f::Green(), false ) );
    m_pResources->AddMaterial( new fw::Material( "VertexColor", m_pResources->GetShader("VertexColor"), nullptr, fw::color4f::White(), false ) );
    m_pResources->AddMaterial( new fw::Material( "SokobanPlayer01", m_pResources->GetShader("Texture"), m_pResources->GetTexture("Sokoban"), fw::color4f::White(), true, info.asVec4() ) );

    // Create a controller.
    m_pPlayerController = new PlayerController();

    m_pActiveScene = new BasicScene( this );
}

void Game::OnShutdown()
{
}

void Game::StartFrame(float deltaTime)
{
    GameCore::StartFrame( deltaTime );

    // Reset the controller.
    m_pPlayerController->StartFrame();

    // Dispatch events.
    m_pEventManager->DispatchAllEvents( deltaTime, this );
}

void Game::OnEvent(fw::Event* pEvent)
{
    // Process events.
    m_pPlayerController->OnEvent( pEvent );
}

void Game::Update(float deltaTime)
{
    GameCore::Update( deltaTime );
}

void Game::Draw()
{
    //bgfx::touch( viewID );

    // Setup time uniforms.
    //float time = (float)fw::GetSystemTimeSinceGameStart();
    //bgfx::setUniform( m_pUniforms->m_Map["u_Time"], &time );

    // Show bgfx debug stats.
    //bgfx::dbgTextClear();
    //if( m_ShowDebugStats )
    //{
    //    bgfx::setDebug( BGFX_DEBUG_STATS );
    //}
    //else
    //{
    //    bgfx::setDebug( BGFX_DEBUG_NONE );
    //}

    GameCore::Draw();
}
