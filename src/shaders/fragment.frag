#version 330 core

uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform vec3 light_position;

in vec3 vert_v_normal;
in vec3 vert_frag_position;

out vec4 frag_color;

void main()
{
    float ambient_strength = 0.1f;
    vec3 ambient = ambient_strength * u_light_color;

    vec3 ray = normalize(light_position - vert_frag_position);
    float cosa = max(dot(ray, vert_v_normal), 0.0f);
    vec3 diffuse = cosa * u_light_color;

    frag_color = vec4(u_object_color * (ambient + diffuse), 1.0);
}
