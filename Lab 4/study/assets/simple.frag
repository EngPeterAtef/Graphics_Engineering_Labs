#version 330
uniform sampler2D tex;

in vec4 vertix_color;

out vec4 frag_color;

void main()
{
    // frag_color = vertix_color ;//+ vec4(0,scale[0],scale[1],1.0);
    frag_color = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
    /*
    tex: the texture to sample from
    ivec2: integer vector2 that constructs the texture coordinate
    0: the mipmap level to sample from (0 is the base level)
    */
}