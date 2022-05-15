//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "DataTypes.h"

void InitTextureFormats();

struct VertexFormat_PosColor
{
    vec3 pos;
    uint32 color;

    static void InitVertexLayout()
    {
        format
            .begin()
            .add( bgfx::Attrib::Position, 3, bgfx::AttribType::Float )
            .add( bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true )
            .end();
    };

    static bgfx::VertexLayout format;
};

struct VertexFormat_PosUV
{
    vec3 pos;
    vec2 uv;

    static void InitVertexLayout()
    {
        format
            .begin()
            .add( bgfx::Attrib::Position,  3, bgfx::AttribType::Float )
            .add( bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float )
            .end();
    };

    static bgfx::VertexLayout format;
};
