#version 330

void main(){
    //the 3 axis area x, y, z: z = depth
    /*
    hna na ktb mkan al points hard-coded but we should learn how to read it
    */
    vec3 position[3]=vec3[3](vec3(-.5,-.5,0),vec3(.5,-.5,0),vec3(0.,.5,0));
    //al vertix shader byt3ml mara le kol point so hna hyt3ml 3 times
    gl_Position=vec4(position[gl_VertexID],1.);
    
}
