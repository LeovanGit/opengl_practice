#include <iostream>

#include <glad/glad.h>  // loads pointers to OpenGL functions at runtime
#include <GLFW/glfw3.h> // creating windows and handle user input

#include "shader.hpp"
#include "camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <glm/glm.hpp> // OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float prev_time = 0;
float delta_time = 0;

double last_mouse_x = 0;
double last_mouse_y = 0;

bool keys[348]; // 348 keys defined in GLFW

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f),
              glm::vec3(0.0f, 0.0f, -1.0f),
              glm::vec3(0.0f, 1.0f, 0.0f),
              5.0f,
              0.1);

//glm::vec3 light_position(1.8f, 0.5f, 3.0f);
glm::vec3 light_position(0.0f, 3.0f, 5.0f);


class Material
{
public:
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

Material gold(glm::vec3(0.24725, 0.1995, 0.0745),
              glm::vec3(0.75164, 0.60648, 0.22648),
              glm::vec3(0.628281, 0.555802, 0.366065),
              4);

Material obsidian(glm::vec3(0.05375, 0.05, 0.06625),
                  glm::vec3(0.18275, 0.17, 0.22525),
                  glm::vec3(0.332741, 0.328634, 0.346435),
                  3);

Material emerald(glm::vec3(0.0215, 0.1745, 0.0215),
                 glm::vec3(0.07568, 0.61424, 0.07568),
                 glm::vec3(0.633, 0.7277811, 0.633),
                 6);

Material pearl(glm::vec3(0.25, 0.20725, 0.20725),
               glm::vec3(1.0, 0.829, 0.829),
               glm::vec3(0.296648),
               0.88);

Material ruby(glm::vec3(0.1745, 0.01175, 0.01175),
              glm::vec3(0.61424, 0.04136, 0.04136),
              glm::vec3(0.727811, 0.626959, 0.626959),
              6);

Material brass(glm::vec3(0.329412, 0.223529, 0.027451),
               glm::vec3(0.780392, 0.569627, 0.113725),
               glm::vec3(0.992157, 0.941176, 0.807843),
               2.1794872);

Material bronze(glm::vec3(0.2125, 0.1275, 0.054),
                glm::vec3(0.714, 0.4284, 0.18144),
                glm::vec3(0.393548, 0.271906, 0.166721),
                2);

Material chrome(glm::vec3(0.25),
                glm::vec3(0.4),
                glm::vec3(0.774597),
                6);

void calc_delta_time()
{
    float current_time = glfwGetTime();
    delta_time = current_time - prev_time;
    prev_time = current_time;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow * window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow * window, double x_offset, double y_offset);

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode * mode = glfwGetVideoMode(monitor);

    GLFWwindow * window = glfwCreateWindow(mode->width,
                                           mode->height,
                                           "OpenGL practice",
                                           monitor,
                                           nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    last_mouse_x = mode->width / 2;
    last_mouse_y = mode->height / 2;
    glfwSetCursorPos(window, last_mouse_x, last_mouse_y);

    Shader shader("shaders/vertex.vert", "shaders/fragment.frag");

    // now light source color not depends from lightning calculations (like other objects)
    // its always bright (1.0f, 1.0f, 1.0f, 1.0f)
    Shader light_shader("shaders/light_vertex.vert", "shaders/light_fragment.frag");
    
    float vertices[] = {
        // vertices           normal
        // up
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,

        // front
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

         // right
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

         // back
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        // left
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

        // down
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f};

    unsigned int VBO, VAO, light_VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &light_VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glBindVertexArray(light_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glm::vec3 cubes[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 1.5f, 0.0f, 0.0f),
        glm::vec3( 3.0f, 0.0f, 0.0f),
        glm::vec3(-1.5f, 0.0f, 0.0f),

        glm::vec3( 0.0f, 1.5f, 0.0f),
        glm::vec3( 1.5f, 1.5f, 0.0f),
        glm::vec3( 3.0f, 1.5f, 0.0f),
        glm::vec3(-1.5f, 1.5f, 0.0f)};

    Material cube_materials[] = {
        gold,
        obsidian,
        emerald,
        pearl,
        ruby,
        brass,
        bronze,
        chrome};

    while (!glfwWindowShouldClose(window))
    {
        calc_delta_time();

        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        camera.move(keys);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        glm::mat4 proj_matrix = glm::perspective(glm::radians(camera.get_fov()),
                                                 float(mode->width) / mode->height,
                                                 0.1f,
                                                 100.0f);

        // draw light source
        light_shader.use();

        glm::mat4 model_matrix = glm::mat4(1.0f);
        model_matrix = glm::translate(model_matrix, light_position);
        model_matrix = glm::scale(model_matrix, glm::vec3(0.2f));

        light_shader.set_uniform_mat4("model_matrix", model_matrix);
        light_shader.set_uniform_mat4("view_matrix", camera.get_view_matrix());
        light_shader.set_uniform_mat4("proj_matrix", proj_matrix);

        glBindVertexArray(light_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // draw objects
        shader.use();

        for (int i = 0; i != 8; ++i)
        {
            model_matrix = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, cubes[i]);
          // model_matrix = glm::rotate(model_matrix, float(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
          // model_matrix = glm::scale(model_matrix, glm::vec3(glm::min(float(glfwGetTime()), 1.0f)));

            shader.set_uniform_mat4("model_matrix", model_matrix);
            shader.set_uniform_mat4("view_matrix", camera.get_view_matrix());
            shader.set_uniform_mat4("proj_matrix", proj_matrix);

            shader.set_uniform_vec3("camera_position", camera.get_position());

            shader.set_uniform_vec3("material.ambient", cube_materials[i].ambient);
            shader.set_uniform_vec3("material.diffuse", cube_materials[i].diffuse);
            shader.set_uniform_vec3("material.specular", cube_materials[i].specular);
            shader.set_uniform_float("material.shininess", cube_materials[i].shininess * 128.0f);

            shader.set_uniform_vec3("light.position", light_position);
            shader.set_uniform_vec3("light.ambient", glm::vec3(1.0f));
            shader.set_uniform_vec3("light.diffuse", glm::vec3(1.0f));
            shader.set_uniform_vec3("light.specular", glm::vec3(1.0f));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS) keys[key] = true;
    else if (action == GLFW_RELEASE) keys[key] = false;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
            break;
        
        case GLFW_KEY_R:
            if (action == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            else if (action == GLFW_RELEASE) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;           

        default:
            break;
    }
}

void mouse_callback(GLFWwindow * window, double mouse_x, double mouse_y)
{
    float x_offset = mouse_x - last_mouse_x;
    float y_offset =  last_mouse_y - mouse_y;
    glfwSetCursorPos(window, last_mouse_x, last_mouse_y);

    camera.rotate(x_offset, y_offset);
}

void scroll_callback(GLFWwindow * window, double x_offset, double y_offset)
{ 
    camera.zoom(y_offset); 
}
