#version 330 core

uniform sampler2D u_major_texture;
uniform sampler2D u_minor_texture;

in vec3 vert_color;
in vec2 vert_texture_coords;

out vec4 frag_color;

void main()
{
    frag_color = mix(texture(u_major_texture, vert_texture_coords),
                     texture(u_minor_texture, vert_texture_coords),
                     0.5f);
}
