#version 330

in vec4 vertix_color;

uniform float time;//uniform variable that will take its value from the c++ code

out vec4 frag_color;

void main()
{
    // frag_color = vec4(0.6706, 0.2431, 0.2431, 1.0);
    vec4 tint = vec4(sin(10* time),sin(10* time),sin(10* time+500),1.0);
    frag_color = vertix_color;
    // frag_color = vertix_color * sin(20* time);
    // frag_color.r = dot(vertix_color,tint);
    // frag_color.g = dot(vertix_color,tint);
    // frag_color.b = dot(vertix_color,tint);
    // frag_color.a = 1.0;
}