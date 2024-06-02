//
// Copyright (c) 2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"
#include "GameComponents.h"
#include "Game.h"
#include "GameDataTypes.h"
#include "Objects/PlayerController.h"

//==============================
// PlayerComponentDefinition
//==============================

void PlayerComponentDefinition::SaveToJSON(fw::GameObject* pObject, nlohmann::json& jComponent, const void* pData)
{
    PlayerData* pPlayerData = (PlayerData*)pData;
    jComponent["Speed"] = pPlayerData->speed;
    
    int controllerIndex = static_cast<Game*>(pObject->GetScene()->GetGameCore())->GetControllerIndex( pPlayerData->pPlayerController );
    jComponent["ControllerIndex"] = controllerIndex;
}

void PlayerComponentDefinition::LoadFromJSON(fw::GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, fw::ResourceManager* pResourceManager)
{
    PlayerData playerData;
    playerData.speed = jComponent["Speed"];

    int controllerIndex = jComponent["ControllerIndex"];
    playerData.pPlayerController = static_cast<Game*>(pObject->GetScene()->GetGameCore())->GetController( controllerIndex );
    
    entity.set<PlayerData>( playerData );
}

void PlayerComponentDefinition::Editor_AddToInspector(flecs::entity entity)
{
    PlayerData& playerData = entity.ensure<PlayerData>();
    if( ImGui::CollapsingHeader( "PlayerData", ImGuiTreeNodeFlags_DefaultOpen ) )
    {
        ImGui::DragFloat( "Speed", &playerData.speed, 0.1f );
    }
    entity.modified<PlayerData>();
}
