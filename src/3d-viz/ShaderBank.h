#ifndef __SHADERBANK_H__
#define __SHADERBANK_H__

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <map>
#include "Shader.h"

enum class ShaderTypes {
    SIMPLE_OBJ_SHADER,
    GROUND_SHADER,
    SIMPLE_2D_SHADER,
    SIMPLE_3D_SHADER,
};

class ShaderBank {
public:
    static ShaderBank& getInstance() {
        static ShaderBank instance;
        return instance;
    }
    static Shader * GetShader(ShaderTypes shaderType);
private:
    ShaderBank();
    static std::map<ShaderTypes, Shader *> Shaders;
};

#endif