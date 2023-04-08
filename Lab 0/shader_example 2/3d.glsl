#iChannel0"pickle_cat.png"
#iChannel0::WrapMode"Clamp"
#iChannel1"picke_cat_heightmap.jpg"

void main(){
    vec2 uv=gl_FragCoord.xy/iResolution.xy;
    float depth=texture(iChannel1,uv).r;
    vec2 offset=-iMouse.xy/iResolution.xy+.5;
    // gl_FragColor=texture(iChannel0,uv+0.05*depth*offset);
    gl_FragColor=texture(iChannel0,uv+0.02*depth *vec2(sin(iTime*10.0),cos(iTime*10.0)));
    gl_FragColor.rgb*=gl_FragColor.a;

    //Rectangle Example

    // vec2 rectcoord=abs(gl_FragCoord.xy-iResolution.xy*.5);
    // if(all(lessThan(rectcoord,vec2(100,100)))){
    //         gl_FragColor=vec4(1.,0.,0.,1.);
    // }else{
    //         gl_FragColor=vec4(0.,0.,0.,1.);
    // }

    //Circle Example

    // float dist=length(gl_FragCoord.xy-iResolution.xy*.5);
    // if(dist<100.){
    //     gl_FragColor=vec4(1.,0.,0.,1.);
    // }else{
    //     gl_FragColor=vec4(0.,0.,0.,1.);
    // }
}
void mainImage(out vec4 fragColor,in vec2 fragCoord)
{
    vec2 rectcoord=abs(fragCoord-iResolution.xy*.5);
    if(all(lessThan(rectcoord,vec2(100.)))){
        fragColor=vec4(1.,0.,0.,1.);
    }else{
        fragColor=vec4(0.,0.,0.,1.);
    }
}

void mainImage2(out vec4 fragColor,in vec2 fragCoord)
{
    float dist=length(fragCoord-iResolution.xy*.5);
    if(dist<100.){
        fragColor=vec4(1.,0.,0.,1.);
    }else{
        fragColor=vec4(0.,0.,0.,1.);
    }
}
