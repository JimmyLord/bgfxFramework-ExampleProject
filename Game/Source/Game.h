//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Framework.h"
#include "DataTypes.h"

class Player;
class PlayerController;

class Game : public fw::GameCore
{
public:
    Game(fw::FWCore& fwCore);
    virtual ~Game() override;

    void Init();
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

protected:
    // Interface.
    fw::ImGuiManager* m_pImGuiManager = nullptr;
    void Editor_DisplayObjectList();

    // Resources.
    std::map<std::string, fw::Mesh*> m_pMeshes;
    std::map<std::string, fw::ShaderProgram*> m_pShaders;
    std::map<std::string, fw::Texture*> m_pTextures;
    std::map<std::string, fw::SpriteSheet*> m_pSpriteSheets;
    std::map<std::string, fw::Material*> m_pMaterials;

    // GameObjects and Helpers.
    PlayerController* m_pPlayerController = nullptr;

    fw::Camera* m_pCamera = nullptr;
    Player* m_pPlayer = nullptr;
    std::vector<fw::GameObject*> m_Objects;

    bool m_ShowDebugStats = false;
};
