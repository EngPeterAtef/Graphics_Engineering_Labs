#version 330

uniform sampler2D tex;

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

void main() {
    frag_color = texture(tex, fs_in.tex_coord);
    if(frag_color.a < 0.5) discard; 
}