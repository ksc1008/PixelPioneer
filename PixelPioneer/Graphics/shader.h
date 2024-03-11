#pragma once
#include <string>
#include <glm/glm.hpp>

class GLShader
{
public:
    unsigned int ID;

    GLShader(const std::string vertexPath, const std::string fragmentPath);

    void use();

    void dispose();

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, glm::vec3 value) const;

    void setModelTransform(glm::mat4 transform) const;

    void setViewTransform(glm::mat4 transform) const;

    void setProjTransform(glm::mat4 transform) const;

    ~GLShader();

private:
    void checkCompileErrors(unsigned int shader, std::string type);
    bool initialized = false;
};
