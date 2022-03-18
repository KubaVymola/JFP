#ifndef __SHADER_H__
#define __SHADER_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <map>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath);
    void RegisterUniform(const std::string& uniformName);
    void Use();
    void Unuse();
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, float v1, float v2, float v3) const;
    void SetMat4(const std::string& name, const glm::mat4& matrix, GLboolean transpose = GL_FALSE) const;
    GLuint getID();
private:
    GLuint compileShader(const char *path, GLenum shaderType);
    GLuint linkShader(GLuint vertexShaderID, GLuint fragmentShaderID);

    std::map<std::string, GLint> _shaderUniformMap;
    GLuint _programID;
};

#endif