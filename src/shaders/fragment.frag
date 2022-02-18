#version 330 core

struct Material
{
    // this is a colors of each lighting component
    vec3 ambient;  // same as object color
    vec3 diffuse;  // color under diffuse light
    vec3 specular; // color of reflected light
    float shininess;
    // P.S. т.е цвет поверхности будет ambient, но на сторону на которую падает свет,
    // итоговый цвет будет состоять из ambient + diffuse. В этом легко убедиться
    // сделав ambient = красный, diffuse = синий, освещенная сторона будет феолетовой, а
    // теневая сторона останется красной
};

struct Light
{
    vec3 position;

    // something like old strength values
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // P.S. это грубо говоря какими цветами стреляет источник света для каждого из компонентов освещения
    // стоит помнить: если к примеру источник света стреляет красным и синим лучом (т.е. феолетовым),
    // но материал поверхности отражает только синий (а красный поглащает), то цвет объекта будет синим.
    // собственно это и зашито в формулу: (1.0, 0.0, 1.0) * (0.0, 0.0, 1.0) = (0.0, 0.0, 1.0)
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
    float shininess = 32;
    vec3 view_ray = normalize(camera_position - vert_frag_position);
    vec3 reflected_ray = reflect(-ray, vert_v_normal);
    float cosb = pow(max(dot(reflected_ray, view_ray), 0.0f), shininess);
    vec3 specular = material.specular * cosb * light.specular;

    frag_color = vec4(ambient + diffuse + specular, 1.0);
}
