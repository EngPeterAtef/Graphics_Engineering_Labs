#version 330

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
};

#define MAX_LIGHTS 8

uniform Light lights[MAX_LIGHTS];
uniform int light_count;

struct Sky{
    vec3 top,horizon,bottom;
};

uniform Sky sky;
/*
this function to calculate the ambient light from 3 directions because the color of the amber light differes
from the source of the light because the sky will give us blue ambient light and the ground will give us green
so we need to know from where we should take our ambient light
if the normal of the surface so we will take the color from the sky(Top)
if the normal of the surface is in the bottom so we will take the color from the ground(Bottom)
*/
vec3 compute_sky_light(vec3 normal){
    vec3 extreme=normal.y>0?sky.top:sky.bottom;//34an a3rf al normal bass le fo2 wla le t7t
    //mix= interpolate between 2 values (horizon and extreme) based on the normal.y * normal.y value (0 to 1)
    /*
    ^ extreme(y or -y)
    |        ^ normal    
    |        |  
    -------------------------> horizion
    */
    return mix(sky.horizon,extreme,normal.y*normal.y);
}

struct Material{
    //texture maps for each property check the slides for more info
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    //the light ali 5arg mn al object nfso han3mlo bel texture da
    sampler2D emissive;
};

uniform Material material;

in Varyings{
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;//point in world space
}fs_in;

out vec4 frag_color;

float lambert(vec3 normal,vec3 world_to_light_direction){
    return max(0.,dot(normal,world_to_light_direction));
}

float phong(vec3 reflected,vec3 view,float shininess){
    return pow(max(0.,dot(reflected,view)),shininess);
}

void main(){
    vec3 normal=normalize(fs_in.normal);
    vec3 view=normalize(fs_in.view);
    
    vec3 ambient_light=compute_sky_light(normal);
    
    vec3 diffuse=texture(material.albedo,fs_in.tex_coord).rgb;
    vec3 specular=texture(material.specular,fs_in.tex_coord).rgb;
    float roughness=texture(material.roughness,fs_in.tex_coord).r;
    vec3 ambient=diffuse*texture(material.ambient_occlusion,fs_in.tex_coord).r;
    vec3 emissive=texture(material.emissive,fs_in.tex_coord).rgb;
    
    //to tell that roughness is between 0.001 and 0.999 so that if it's larger than 0.999 it will be 0.999
    // and same for 0.001. we did that to avoid division by zero and to avoid shininess to be zero
    float shininess=(2./pow(clamp(roughness,.001,.999),4.))-2.;
    
    vec3 world_to_light_dir;
    vec3 color=emissive+ambient_light*ambient;
    
    for(int light_idx=0;light_idx<min(MAX_LIGHTS,light_count);light_idx++){
        Light light=lights[light_idx];
        float attenuation=1.;
        if(light.type==DIRECTIONAL){
            world_to_light_dir=-light.direction;
        }else{
            // in case of spot and point light we calculate the light direction as the vector from the point to the light position
            world_to_light_dir=light.position-fs_in.world;
            float d=length(world_to_light_dir);//magnitude
            world_to_light_dir/=d;//normalize
            //attenuation = 1/ ad^2+ad+a where a is light.attenuation
            attenuation=1./dot(light.attenuation,vec3(d*d,d,1.));
            
            if(light.type==SPOT){
                //in case of spot light there is a difference between the vector from the point to the light source and
                //the direction of the spot light itself
                //we calculate the angle betweem them to know the attenuation
                float angle=acos(dot(light.direction,-world_to_light_dir));
                //smooth step is like sigmoid function
                //outer angle(2a2l value)  inner(akbr value)  input(angle)
                attenuation*=smoothstep(light.cone_angles.y,light.cone_angles.x,angle);
            }
        }
        
        vec3 computed_diffuse=light.color*diffuse*lambert(normal,world_to_light_dir);
        
        vec3 reflected=reflect(-world_to_light_dir,normal);
        vec3 computed_specular=light.color*specular*phong(reflected,view,shininess);
        
        color+=(computed_diffuse+computed_specular)*attenuation;
        
    }
    
    frag_color=vec4(color,1.);
    // frag_color = vec4(fs_in.normal, 1.0);
}