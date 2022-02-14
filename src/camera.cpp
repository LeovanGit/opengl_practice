#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"

extern float delta_time;

Camera::Camera(glm::vec3 position, glm::vec3 ray, glm::vec3 up,
               float move_speed, float rotation_speed) :
    position(position), ray(ray), up(up),
    move_speed(move_speed), rotation_speed(rotation_speed),
    yaw(-90.0f), pitch(0),
    fov(45.0f) {}

glm::mat4 Camera::get_view_matrix() const { return glm::lookAt(position, position + ray, up); }

float Camera::get_fov() const { return fov; }

glm::vec3 Camera::get_position() const { return position; }

void Camera::move(bool *keys)
{
    if (keys[GLFW_KEY_W])
        position += ray * move_speed * delta_time;
    if (keys[GLFW_KEY_S]) 
        position -= ray * move_speed * delta_time;
    if (keys[GLFW_KEY_A]) 
        position -= glm::normalize(glm::cross(ray, up)) * move_speed * delta_time;
    if (keys[GLFW_KEY_D])
        position += glm::normalize(glm::cross(ray, up)) * move_speed * delta_time;
}

void Camera::rotate(float x_offset, float y_offset)
{
    yaw   += x_offset * rotation_speed;
    pitch += y_offset * rotation_speed;
    
    if (pitch > 89.0f) pitch = 89.0f;
    else if (pitch < -89.0f) pitch = -89.0f;

    ray.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    ray.y = glm::sin(glm::radians(pitch));
    ray.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    ray = glm::normalize(ray);
}

void Camera::zoom(float y_offset)
{
    fov -= float(y_offset);
    if (fov < 1.0f) fov = 1.0f;
    else if (fov > 45.0f) fov = 45.0f;
}

