#version 330 core

layout (location = 0) in vec3 v_coords;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texture_coords;

out vec3 vert_color;
out vec2 vert_texture_coords;

void main()
{
    gl_Position = vec4(v_coords.x * 0.5625, v_coords.y, v_coords.z, 1.0f);
    vert_color = v_color;
    vert_texture_coords = v_texture_coords;
}
