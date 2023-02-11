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
    if( bgfx::isValid( m_FBO ) )
    {
        bgfx::destroy( m_FBO );
    }

    if( bgfx::isValid( m_FBOTexture ) )
    {
        bgfx::destroy( m_FBOTexture );
    }

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

    delete m_pPlayerController;

    delete m_pEventManager;

    delete m_pImGuiManager;
}

void Game::Init()
{
    m_pImGuiManager = new fw::ImGuiManager( &m_FWCore, 1 );

    // General renderer settings.
    bgfx::setViewClear( 0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( 0, 0, 0, m_FWCore.GetWindowClientWidth(), m_FWCore.GetWindowClientHeight() );

    m_pEventManager = new fw::EventManager();

    // Create uniforms.
    m_pUniforms = new fw::Uniforms();

    // Create vertex formats.
    InitVertexFormats();

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

    m_pActiveScene = new BasicScene( this );

    // Create an FBO along with a texture to render to.
    // TODO: Don't limit this to a 2048x2048 texture. Have it resize if the window is resized to a larger size.
    m_FBOTexture = bgfx::createTexture2D( m_GameTextureSize.x, m_GameTextureSize.y, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL );
    bgfx::TextureHandle fbTextures[] = { m_FBOTexture };
    m_FBO = bgfx::createFrameBuffer( 1, fbTextures, true );
}

void Game::OnShutdown()
{
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
}

void Game::Update(float deltaTime)
{
    m_pActiveScene->Update( deltaTime );
    
    Editor_DisplayObjectList();
}

void Game::Draw()
{
    int viewID = 0;

    // Render the scene into an FBO.
    bgfx::setViewFrameBuffer( viewID, m_FBO );
    bgfx::setViewRect( viewID, 0, 0, m_GameWindowSize.x, m_GameWindowSize.y );
    bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0 );
    
    //bgfx::touch( viewID );

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform( m_pUniforms->m_Map["u_Time"], &time );

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

    // Draw our main view in a window.
    if( ImGui::Begin("Game view") )
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 size = contentMax - contentMin;
        m_GameWindowSize = ivec2( (int)size.x, (int)size.y );
        if( m_GameWindowSize.x > m_GameTextureSize.x ) { m_GameWindowSize.x = m_GameTextureSize.x; }
        if( m_GameWindowSize.y > m_GameTextureSize.y ) { m_GameWindowSize.y = m_GameTextureSize.y; }

        m_pActiveScene->Draw( viewID );

        vec2 uvMax = vec2( (float)m_GameWindowSize.x / m_GameTextureSize.x, (float)m_GameWindowSize.y / m_GameTextureSize.y );
        ImGui::Image( fw::imguiTexture(m_FBOTexture), ImVec2( (float)m_GameWindowSize.x, (float)m_GameWindowSize.y ), ImVec2(0,0), uvMax );
    }
    ImGui::End();

    m_pImGuiManager->EndFrame();
}

void Game::Editor_DisplayObjectList()
{
    ImGui::Begin( "Object List" );

    m_pActiveScene->Editor_DisplayObjectList();

    ImGui::End(); // "Object List"
}
