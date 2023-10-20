$input v_texcoord0, v_texcoord1

#include <bgfx_shader.sh>

SAMPLER2D( u_TextureColor, 0 );
SAMPLER2D( u_TextureNoise, 1 );

uniform vec4 u_ControlPerc;

void main()
{
    vec4 color = texture2D( u_TextureColor, v_texcoord0 );
    float noise = texture2D( u_TextureNoise, v_texcoord1 ).r;
    
    float alpha = color.a * (noise+1 - (u_ControlPerc*2.0));
    gl_FragColor = vec4( color.rgb, alpha );
}
