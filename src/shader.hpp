#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(std::string vert_path, std::string frag_path);

    void use();

    unsigned int get_id();

    void set_uniform_mat4(const char * uniform_name, glm::mat4 matrix);

private:
    std::string read_from_file(std::string path);

    void check_error(int type, unsigned int shader_id = -1);

    unsigned int id;
};

#endif
