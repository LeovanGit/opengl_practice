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
    // We need to transform our normal into global space like we do with objects:
    // vert_v_normal = mat3(model_matrix) * normalize(v_normal);
    // this formula has one problem: after non-uniform scale normal don't
    // perpendicular to surface more. Solution: we need to use normal_matrix:
    mat3 normal_matrix = mat3(transpose(inverse(model_matrix)));
    vert_v_normal = normalize(normal_matrix * v_normal);
    // WARNING! inverse is a costly operation for shaders,
    // better to calculate normal matrix on CPU

    vert_frag_position = vec3(model_matrix * vec4(v_coords, 1.0f)); // in global space

    gl_Position = proj_matrix * 
                  view_matrix * 
                  model_matrix * 
                  vec4(v_coords, 1.0f);
}
