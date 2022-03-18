#include "ShaderBank.h"

#include <iostream>
#include <string>
#include "jfp_config.h"

std::map<ShaderTypes, Shader *> ShaderBank::Shaders;

ShaderBank::ShaderBank() {
    std::cout << "Shader path: " << SHADER_PATH_ABSOLUTE << std::endl;

    // ==== Initialize all shaders ====
    ShaderBank::Shaders[ShaderTypes::GROUND_SHADER]
        = new Shader(SHADER_PATH_ABSOLUTE "groundShader.vert",
                     SHADER_PATH_ABSOLUTE "groundShader.frag");

    ShaderBank::Shaders[ShaderTypes::SIMPLE_OBJ_SHADER]
        = new Shader(SHADER_PATH_ABSOLUTE "simpleObjShader.vert",
                     SHADER_PATH_ABSOLUTE "simpleObjShader.frag");

    ShaderBank::Shaders[ShaderTypes::SIMPLE_2D_SHADER]
        = new Shader(SHADER_PATH_ABSOLUTE "simple2DShader.vert",
                     SHADER_PATH_ABSOLUTE "simple2DShader.frag");

    ShaderBank::Shaders[ShaderTypes::SIMPLE_3D_SHADER]
        = new Shader(SHADER_PATH_ABSOLUTE "simple3DShader.vert",
                     SHADER_PATH_ABSOLUTE "simple3DShader.frag");
    // ==== End Initialize all shaders ====
}

Shader * ShaderBank::GetShader(ShaderTypes shaderType) {
    if (!ShaderBank::Shaders.count(shaderType)) return nullptr;

    return ShaderBank::Shaders.find(shaderType)->second;
}