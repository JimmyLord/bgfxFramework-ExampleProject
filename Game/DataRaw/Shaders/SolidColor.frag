#include <bgfx_shader.sh>

uniform vec4 u_DiffuseColor;

void main()
{
    gl_FragColor = u_DiffuseColor;
}
