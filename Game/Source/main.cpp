//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    //fw::FWCore fwCore( 600, 600, bgfx::RendererType::Direct3D9 );
    //fw::FWCore fwCore( 600, 600, bgfx::RendererType::Direct3D11 );
    //fw::FWCore fwCore( 600, 600, bgfx::RendererType::Direct3D12 );
    //fw::FWCore fwCore( 600, 600, bgfx::RendererType::OpenGLES );
    //fw::FWCore fwCore( 600, 600, bgfx::RendererType::OpenGL );
    fw::FWCore fwCore( 600, 600, bgfx::RendererType::Vulkan );
    //fw::FWCore fwCore( 600, 600, bgfx::RendererType::WebGPU );

    ////untested fw::FWCore fwCore( 600, 600, bgfx::RendererType::Agc );
    ////untested fw::FWCore fwCore( 600, 600, bgfx::RendererType::Gnm );
    ////untested fw::FWCore fwCore( 600, 600, bgfx::RendererType::Metal );
    ////untested fw::FWCore fwCore( 600, 600, bgfx::RendererType::Nvn );

    Game game( fwCore );

    fwCore.Run( game );

    fwCore.Shutdown();

    return 0;
}
