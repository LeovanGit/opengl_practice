#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
    Shader(std::string vert_path, std::string frag_path);

    void use();

    unsigned int get_id();

private:
    std::string read_from_file(std::string path);

    void check_error(int type, unsigned int shader_id = -1);

    unsigned int id;
};

#endif
