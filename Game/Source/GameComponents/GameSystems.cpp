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

void System_UpdateAllPlayerComponents(fw::ComponentManager* pComponentManager, float deltaTime)
{
    flecs::world& world = pComponentManager->GetFlecsWorld();

    world.each<>(
        [deltaTime](fw::TransformData& transformData, PlayerData& playerData)
        {
            if( playerData.pPlayerController == nullptr )
                return;

            float speed = 4.0f;

            vec2 dir;

            if( playerData.pPlayerController->IsHeld( PlayerController::Mask::Up ) )    { dir.y +=  1; }
            if( playerData.pPlayerController->IsHeld( PlayerController::Mask::Down ) )  { dir.y += -1; }
            if( playerData.pPlayerController->IsHeld( PlayerController::Mask::Left ) )  { dir.x += -1; }
            if( playerData.pPlayerController->IsHeld( PlayerController::Mask::Right ) ) { dir.x +=  1; }

            if( playerData.pPlayerController->WasPressed( PlayerController::Mask::Action ) )
            {
                transformData.position.x = fw::Random::Float( 0.0f, 10.0f );
                transformData.position.y = fw::Random::Float( 0.0f, 10.0f );
            }

            dir.Normalize();

            transformData.position += dir * speed * deltaTime;
        }
    );
}
