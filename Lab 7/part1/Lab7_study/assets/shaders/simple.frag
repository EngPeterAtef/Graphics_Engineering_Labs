#version 330

#define DIRECTIONAL 0
#define POINT       1
#define SPOT        2

struct Light {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
};

in Varyings{
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
}fs_in;

out vec4 frag_color;

float lambert(vec3 light_dir,vec3 normal){
    return max(dot(light_dir,normal),0.);//to avoid negative values
}

float phong(vec3 reflect_dir,vec3 view_dir,float shininess){
    return pow(max(dot(reflect_dir,view_dir),0.),shininess);
}
void main(){
    vec3 normal=normalize(fs_in.normal);
    vec3 view=normalize(fs_in.view);
    vec3 light_dir=normalize(vec3(1.,1.,1.));//direction of the light
    
    vec3 light_ambient=vec3(.1,.2,.3);//ambient color
    vec3 material_ambient=vec3(1.,.9,.7);//color of the object out of the light
    vec3 ambient=light_ambient*material_ambient*1;//m4 bndrb fe factor like diffuse 34an al light bykon same intensity from all 
    // this 1 34an kolal directions leha nfs al intensity of the ambient light
    
    vec3 light_diffuse=vec3(1.,.9,.7);//color of the object in the light
    vec3 material_diffuse=vec3(.5,.4,.3);//color of the object out of the light
    float cos_theta=lambert(light_dir,fs_in.normal);
    vec3 diffuse=light_diffuse*material_diffuse*cos_theta;
    //specular
    vec3 reflect_dir=reflect(-light_dir,normal);//reflect_dir = 2(N · L)N - L where L = -
    vec3 light_specular=vec3(0.4275, 1.0, 0.6667);//color of the object in the light
    vec3 material_specular=vec3(1.,.9,.8);//color of the object in the light
    float material_shininess=10.;//shininess of the object
    vec3 specular=light_specular*material_specular*phong(reflect_dir,view,material_shininess);

    //add all the components
    frag_color=vec4(ambient+diffuse+specular,1.);
}