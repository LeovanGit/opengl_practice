#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 camera_position;

in vec3 vert_v_normal;
in vec3 vert_frag_position;

out vec4 frag_color;

void main()
{
    // ambient
    vec3 ambient = material.ambient * light.ambient;

    // diffuse
    vec3 ray = normalize(light.position - vert_frag_position);
    float cosa = max(dot(ray, vert_v_normal), 0.0f);
    vec3 diffuse = cosa * material.diffuse * light.diffuse;

    // specular
    vec3 view_ray = normalize(camera_position - vert_frag_position);
    vec3 reflected_ray = reflect(-ray, vert_v_normal);
    float cosb = pow(max(dot(reflected_ray, view_ray), 0.0f), material.shininess);
    vec3 specular = material.specular * cosb * light.specular;

    frag_color = vec4(ambient + diffuse + specular, 1.0);
}
