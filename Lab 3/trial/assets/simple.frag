#version 330

in vec4 vertix_color;

out vec4 frag_color;

void main()
{
    frag_color = vertix_color ;//+ vec4(0,scale[0],scale[1],1.0);
}