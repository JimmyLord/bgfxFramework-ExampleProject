$input a_position, a_texcoord0
$output v_texcoord0

#include <bgfx_shader.sh>

uniform vec4 u_Time;
uniform vec4 u_UVScaleOffset;

void main()
{
    // Code to test time uniform is working.
    vec3 objectSpacePos = a_position;
    //objectSpacePos.y += sin(u_Time.x + a_position.x) * 0.1f;

    gl_Position = mul( u_modelViewProj, vec4(objectSpacePos, 1.0) );

    v_texcoord0 = a_texcoord0 * u_UVScaleOffset.xy + u_UVScaleOffset.zw;
}
