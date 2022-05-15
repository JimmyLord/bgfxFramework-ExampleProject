$input a_position

#include <bgfx_shader.sh>

uniform vec4 u_Time;

void main()
{
    // Code to test time uniform is working.
    vec3 objectSpacePos = a_position;
    //objectSpacePos.y += sin(u_Time.x + a_position.x) * 0.1f;

    gl_Position = mul( u_modelViewProj, vec4(objectSpacePos, 1.0) );
}
