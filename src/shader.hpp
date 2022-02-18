#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(std::string vert_path, std::string frag_path);

    void use() const;

    unsigned int get_id() const;

    void set_uniform_float(const std::string &uniform_name, float value);

    void set_uniform_vec3(const std::string &uniform_name, glm::vec3 vector);

    void set_uniform_mat4(const std::string &uniform_name, glm::mat4 matrix);

private:
    std::string read_from_file(std::string path);

    void check_error(int type, unsigned int shader_id = -1);

    unsigned int id;
};

#endif
