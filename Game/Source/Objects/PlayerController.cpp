//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "PlayerController.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::StartFrame()
{
    m_OldFlags = m_Flags;
}

void PlayerController::OnEvent(fw::Event* pEvent)
{
    if( pEvent->GetType() == fw::InputEvent::GetStaticEventType() )
    {
        fw::InputEvent* pInputEvent = static_cast<fw::InputEvent*>( pEvent );
        
        if( pInputEvent->GetDeviceType() == fw::InputEvent::DeviceType::Keyboard )
        {
            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Pressed )
            {
                if( pInputEvent->GetKeyCode() == 'W' ) { m_Flags |= Mask::Up; }
                if( pInputEvent->GetKeyCode() == 'S' ) { m_Flags |= Mask::Down; }
                if( pInputEvent->GetKeyCode() == 'A' ) { m_Flags |= Mask::Left; }
                if( pInputEvent->GetKeyCode() == 'D' ) { m_Flags |= Mask::Right; }
                if( pInputEvent->GetKeyCode() == 'Z' ) { m_Flags |= Mask::Action; }
            }

            if( pInputEvent->GetDeviceState() == fw::InputEvent::DeviceState::Released )
            {
                if( pInputEvent->GetKeyCode() == 'W' ) { m_Flags &= ~Mask::Up; }
                if( pInputEvent->GetKeyCode() == 'S' ) { m_Flags &= ~Mask::Down; }
                if( pInputEvent->GetKeyCode() == 'A' ) { m_Flags &= ~Mask::Left; }
                if( pInputEvent->GetKeyCode() == 'D' ) { m_Flags &= ~Mask::Right; }
                if( pInputEvent->GetKeyCode() == 'Z' ) { m_Flags &= ~Mask::Action; }
            }
        }
    }
}

bool PlayerController::IsHeld(Mask mask)
{
    return (m_Flags & mask) != 0;
}

bool PlayerController::WasPressed(Mask mask)
{
    return ((m_Flags & mask) != 0) && ((m_OldFlags & mask) == 0);
}

bool PlayerController::WasReleased(Mask mask)
{
    return ((m_Flags & mask) == 0) && ((m_OldFlags & mask) != 0);
}
