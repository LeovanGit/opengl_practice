#version 330 core

layout (location = 0) in vec3 v_coords;

uniform mat4 model_matrix; // local space -> world (global) space
uniform mat4 view_matrix; // global space -> view space
uniform mat4 proj_matrix; // view space -> clip space (projection 3D -> 2D)

void main()
{
    gl_Position = proj_matrix * 
                  view_matrix * 
                  model_matrix * 
                  vec4(v_coords, 1.0f);
}
