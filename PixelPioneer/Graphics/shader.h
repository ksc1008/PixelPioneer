#pragma once
#include <string>

class GLShader
{
public:
    unsigned int ID;

    GLShader(const char* vertexPath, const char* fragmentPath);

    void use();

    void dispose();

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    ~GLShader();

private:
    void checkCompileErrors(unsigned int shader, std::string type);

};
