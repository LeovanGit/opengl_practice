#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 ray, glm::vec3 up,
           float move_speed, float rotation_speed);

    glm::mat4 get_view_matrix() const;

    float get_fov();

    void move(bool *keys);

    void rotate(float x_offset, float y_offset);

    void zoom(float y_offset);

private:
    glm::vec3 position;
    glm::vec3 ray;
    glm::vec3 up;

    float move_speed;
    float rotation_speed;

    float yaw;
    float pitch;

    float fov;
};

#endif
