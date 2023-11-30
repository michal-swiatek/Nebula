//
// Created by michal-swiatek on 30.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "platform/OpenGL/OpenGLShader.h"

namespace nebula::rendering {

    OpenGLShader::OpenGLShader(
        const std::string& name,
        const ShaderTemplate& shader_template
    ) :
            Shader(name, shader_template)
    {

    }

    OpenGLShader::~OpenGLShader()
    {

    }

    void OpenGLShader::bind()
    {

    }

    void OpenGLShader::unbind()
    {

    }

    void* OpenGLShader::getStageHandle(ShaderStage stage)
    {

    }

    std::vector<char> OpenGLShader::loadBinaryFile(const std::string& path)
    {
        throw std::runtime_error("OpenGL shader does not support binary files!");
    }

}
