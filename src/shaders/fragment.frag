#version 330 core

uniform sampler2D u_texture;

in vec3 vert_color;
in vec2 vert_texture_coords;

out vec4 frag_color;

void main()
{
    frag_color = texture(u_texture, vert_texture_coords);
}
