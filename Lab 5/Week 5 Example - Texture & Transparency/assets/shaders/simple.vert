#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;

uniform mat4 MVP;

out Varyings {
    vec4 color;
    vec2 tex_coord;
} vs_out;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
}