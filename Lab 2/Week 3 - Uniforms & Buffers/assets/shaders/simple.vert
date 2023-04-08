#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform float time;

out vec4 vertex_color;

void main() {
    gl_Position = vec4(position, 1.0);
    vertex_color = color;
}