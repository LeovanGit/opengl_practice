#version 330 core

layout (location = 0) in vec3 v_coords;
layout (location = 1) in vec3 v_normal;

uniform mat4 model_matrix; // local space -> world (global) space
uniform mat4 view_matrix; // global space -> view space
uniform mat4 proj_matrix; // view space -> clip space (projection 3D -> 2D)

out vec3 vert_v_normal;
out vec3 vert_frag_position;

void main()
{
    mat3 normal_matrix = mat3(transpose(inverse(model_matrix)));
    vert_v_normal = normalize(normal_matrix * v_normal);

    vert_frag_position = vec3(model_matrix * vec4(v_coords, 1.0f)); // in global space

    gl_Position = proj_matrix * 
                  view_matrix * 
                  model_matrix * 
                  vec4(v_coords, 1.0f);
}
