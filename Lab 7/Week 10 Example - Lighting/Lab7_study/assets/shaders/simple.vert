#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;
layout(location = 3) in vec3 normal;

uniform mat4 VP;
uniform vec3 camera_position;
uniform mat4 M;
uniform mat4 M_IT;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

void main() {

    vec3 position_world = (M * vec4(position, 1.0)).xyz; //from object to world space
    gl_Position = VP * vec4(position_world, 1.0); //from world to clip space
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
    // we need to normalize the normal to calculate the diffuse in the fragment shader
    //also the normal transformed using ((M)^-1).T
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz); //.xyz to discard the w component not needed
    //the position of the camera in world space "view vector"
    vs_out.view = camera_position - position_world;
    vs_out.world = position_world;
}
