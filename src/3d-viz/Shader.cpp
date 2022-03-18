#include "Shader.h"

#include <iostream>
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    GLuint vertexShaderID = compileShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShaderID = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    

    _programID = linkShader(vertexShaderID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

GLuint Shader::compileShader(const char *path, GLenum shaderType) {
    std::string shaderCode;
    std::ifstream shaderFile;

    printf("%s\n", path);

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(path);
        std::stringstream shaderStream;

        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        shaderCode = shaderStream.str();
    } catch (std::ifstream::failure e) {
        printf("ERROR::SHADER::%d::FILE_NOT_READ_SUCCESFULLY\n", shaderType);
    }

    const char *shaderCCode = shaderCode.c_str();

    GLuint shaderID;
    GLint success;
    GLchar infoLog[512];


    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCCode, nullptr);
    glCompileShader(shaderID);

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        printf("ERROR::SHADER::%d::COMPILATION::FAILED\n%s\n", shaderType, infoLog);
    }

    return shaderID;
}

void Shader::RegisterUniform(const std::string& uniformName) {
    _shaderUniformMap[uniformName] = glGetUniformLocation(getID(), uniformName.c_str());
}

GLuint Shader::linkShader(GLuint vertexShaderID, GLuint fragmentShaderID) {
    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        printf("ERROR::SHADER::LINK::FAILED\n%s\n", infoLog);
    }

    return programID;
}

GLuint Shader::getID() {
    return _programID;
}

void Shader::Use() {
    glUseProgram(_programID);
}

void Shader::Unuse() {
    glUseProgram(0);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(_programID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(_programID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_programID, name.c_str()), value);
}

void Shader::SetVec3(const std::string &name, float v1, float v2, float v3) const {
    glUniform3f(glGetUniformLocation(_programID, name.c_str()), v1, v2, v2);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix, GLboolean transpose) const {
    glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, transpose, glm::value_ptr(matrix));
}