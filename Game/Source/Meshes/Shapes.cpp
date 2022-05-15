//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "VertexFormats.h"
#include "Shapes.h"

//=======================
// Triangle
//=======================
static const VertexFormat_PosColor g_TriangleVerts[] =
{
    { vec3( 0.0f, 0.5f, 0.0f), 0x000000ff }, // Top
    { vec3(-0.5f,-0.5f, 0.0f), 0x0000ff00 }, // Bottom-left
    { vec3( 0.5f,-0.5f, 0.0f), 0x00ff0000 }, // Bottom-right
};
static const uint16 g_TriangleIndices[] =
{
    0, 2, 1,
};
fw::Mesh* CreateTriangleMesh()
{
    return new fw::Mesh( VertexFormat_PosColor::format, g_TriangleVerts, sizeof(g_TriangleVerts), g_TriangleIndices, sizeof(g_TriangleIndices) );
}

//=======================
// Square
//=======================
static const VertexFormat_PosColor g_SquareVerts[] =
{
    { vec3(-0.5f, 0.5f, 0.0f), 0x0000ff00 }, // Top-left
    { vec3(-0.5f,-0.5f, 0.0f), 0x00000000 }, // Bottom-left
    { vec3( 0.5f,-0.5f, 0.0f), 0x00000000 }, // Bottom-right

    { vec3(-0.5f, 0.5f, 0.0f), 0x0000ff00 }, // Top-left
    { vec3( 0.5f,-0.5f, 0.0f), 0x00000000 }, // Bottom-right
    { vec3( 0.5f, 0.5f, 0.0f), 0x0000ff00 }, // Top-right
};
static const uint16 g_SquareIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSquareMesh()
{
    return new fw::Mesh( VertexFormat_PosColor::format, g_SquareVerts, sizeof(g_SquareVerts), g_SquareIndices, sizeof(g_SquareIndices) );
}

//=======================
// Sprite
//=======================
static const VertexFormat_PosUV g_SpriteVerts[] =
{
    { vec3(-0.5f, 0.5f, 0.0f), vec2(0,1) }, // Top-left
    { vec3(-0.5f,-0.5f, 0.0f), vec2(0,0) }, // Bottom-left
    { vec3( 0.5f,-0.5f, 0.0f), vec2(1,0) }, // Bottom-right

    { vec3(-0.5f, 0.5f, 0.0f), vec2(0,1) }, // Top-left
    { vec3( 0.5f,-0.5f, 0.0f), vec2(1,0) }, // Bottom-right
    { vec3( 0.5f, 0.5f, 0.0f), vec2(1,1) }, // Top-right
};
static const uint16 g_SpriteIndices[] =
{
    0,2,1, 3,5,4
};
fw::Mesh* CreateSpriteMesh()
{
    return new fw::Mesh( VertexFormat_PosUV::format, g_SpriteVerts, sizeof(g_SpriteVerts), g_SpriteIndices, sizeof(g_SpriteIndices) );
}
