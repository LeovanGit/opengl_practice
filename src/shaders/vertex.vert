#version 330 core

layout (location = 0) in vec3 v_coords;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_texture_coords;

uniform mat4 model_matrix; // local space -> world (global) space
uniform mat4 view_matrix; // global space -> view space
uniform mat4 proj_matrix; // view space -> clip space (projection 3D -> 2D)

out vec3 vert_color;
out vec2 vert_texture_coords;

void main()
{
    // texture transform automatically with position!
    gl_Position = proj_matrix * 
                  view_matrix * 
                  model_matrix * 
                  vec4(v_coords, 1.0f);
    vert_color = v_color;
    vert_texture_coords = v_texture_coords;
}
