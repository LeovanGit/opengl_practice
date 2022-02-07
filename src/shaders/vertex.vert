#version 330 core

layout (location = 0) in vec3 coordinates;
layout (location = 1) in vec3 color;

out vec3 vert_color;

void main()
{
    gl_Position = vec4(coordinates, 1.0f);
    vert_color = color;
}
