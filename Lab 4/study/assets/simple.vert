#version 330
layout(location = 0 )in vec3 position;//3 points in the shader
layout(location = 1 )in vec4 color; //hygelo mn el c++ code
layout(location = 2 )in vec2 tex_coord; //hygelo mn el c++ code
//kda a7na sabtna al location bta3 al position w al color le kol al shaders 34an ma7tg4 aktb 
//nfs al code fel c++ le kol shader
// out vec4 vertix_color;

out Varyings{
    vec4 color;
    vec2 tex_coord;
}vs_out;

uniform mat4 MVP;//matrix 4x4
//MVP: model view projection
//model: 3shan a3ml rotation w translation w scale
//view: 3shan a3ml camera
//projection: 3shan a3ml perspective

void main(){
    //al vertix shader byt3ml mara le kol point so hna hyt3ml 3 times
    // gl_VertexID: is an id for earch vertix (each point in tha shader)
    // gl_Position=vec4(position*vec3(sin(time),cos(time),cos(time)),1.0);
    gl_Position = MVP * vec4(position, 1.0);
    // vertix_color=color;//now we set the color for each point
    vs_out.color=color;
    vs_out.tex_coord=tex_coord;
    
}
