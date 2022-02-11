#include <iostream>

#include <glad/glad.h>  // loads pointers to OpenGL functions at runtime
#include <GLFW/glfw3.h> // creating windows and handle user input

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#include <glm/glm.hpp> // OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float move_speed = 5.0f;
float rotation_speed = 0.15f;

glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
glm::vec3 camera_ray(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up(0.0f, 1.0f, 0.0f);

float prev_time = 0;
float delta_time = 0;

double last_mouse_x = 0;
double last_mouse_y = 0;
float yaw = -90.0f;
float pitch = 0;

bool keys[348]; // 348 keys defined in GLFW

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
void move();
void rotate();

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
    
    float vertices[] = {
        // vertices           colors           texture_coords
        // up
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        // front
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

         // right
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

         // back
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        // left
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        // down
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f};

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); 

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    // Textures(s, t, r)
    stbi_set_flip_vertically_on_load(true); 

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int t_width, t_height, nr_channels;
    unsigned char *texture_data = stbi_load("../assets/textures/texture_lava.png",
                                            &t_width, 
                                            &t_height,
                                            &nr_channels,
                                            0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    glm::vec3 cube_coords[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(-2.4f, -0.4f, -3.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(2.0f,  5.0f, -15.0f)};

    // transformations
    glm::mat4 proj_matrix = glm::perspective(glm::radians(45.0f),
                                             float(mode->width) / mode->height,
                                             0.1f,
                                             100.0f);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        calc_delta_time();

        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        
        move();
        rotate();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        // transformations
        glm::mat4 view_matrix = glm::lookAt(camera_pos, camera_pos + camera_ray, camera_up);
        glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "view_matrix"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(view_matrix));

        glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "proj_matrix"),
                           1,
                           GL_FALSE,
                           glm::value_ptr(proj_matrix));

        for (int i = 0; i != 5; ++i)
        {
            glm::mat4 model_matrix = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, cube_coords[i]);
            model_matrix = glm::rotate(model_matrix, glm::radians(i * 15.0f), glm::vec3(0.3, 1.0, 0.6));
            glUniformMatrix4fv(glGetUniformLocation(shader.get_id(), "model_matrix"),
                               1,
                               GL_FALSE,
                               glm::value_ptr(model_matrix));            
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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
    
    if (key == GLFW_KEY_R && action == GLFW_PRESS) 
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_R && action == GLFW_RELEASE) 
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (action == GLFW_PRESS) 
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

void move()
{
    if (keys[GLFW_KEY_W])
        camera_pos += camera_ray * move_speed * delta_time;
    if (keys[GLFW_KEY_S]) 
        camera_pos -= camera_ray * move_speed * delta_time;
    if (keys[GLFW_KEY_A]) 
        camera_pos -= glm::normalize(glm::cross(camera_ray, camera_up)) * move_speed * delta_time;
    if (keys[GLFW_KEY_D])
        camera_pos += glm::normalize(glm::cross(camera_ray, camera_up)) * move_speed * delta_time;
}

void mouse_callback(GLFWwindow * window, double mouse_x, double mouse_y)
{
    float offset_x = mouse_x - last_mouse_x;
    float offset_y =  last_mouse_y - mouse_y;
    glfwSetCursorPos(window, last_mouse_x, last_mouse_y);

    yaw += offset_x * rotation_speed;
    pitch += offset_y * rotation_speed;
    
    if (pitch > 89.0f) pitch = 89.0f;
    else if (pitch < -89.0f) pitch = -89.0f;
}

// Euler angles
void rotate()
{
    camera_ray.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    camera_ray.y = glm::sin(glm::radians(pitch));
    camera_ray.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    camera_ray = glm::normalize(camera_ray);
}
