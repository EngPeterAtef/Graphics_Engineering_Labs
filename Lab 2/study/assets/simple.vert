#version 330
out vec4 vertix_color;
layout(location = 0 )in vec3 position;//3 points in the shader
// layout(location = 1 )in vec4 color; //hygelo mn el c++ code
//kda a7na sabtna al location bta3 al position w al color le kol al shaders 34an ma7tg4 aktb 
//nfs al code fel c++ le kol shader
uniform float time;//uniform variable that will take its value from the c++ code
void main(){
    //al vertix shader byt3ml mara le kol point so hna hyt3ml 3 times
    // gl_VertexID: is an id for earch vertix (each point in tha shader)
    // gl_Position=vec4(position*vec3(sin(time),cos(time),cos(time)),1.0);
    vec4 color;
    if(gl_VertexID==0){
        color=vec4(1.0,0.0,0.0,1.0);
    }
    else if(gl_VertexID==1){    
        color=vec4(0.0,1.0,0.0,1.0);
    }
    else if(gl_VertexID==2){
        color=vec4(0.0,0.0,1.0,1.0);
    }
    gl_Position = vec4(position, 1.0);
    vertix_color=color;//now we set the color for each point
    
}
