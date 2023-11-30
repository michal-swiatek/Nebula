//
// Created by michal-swiatek on 30.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "rendering/Shader.h"

namespace nebula::rendering {

    class OpenGLShader final : public Shader
    {
    public:
        OpenGLShader(const std::string& name, const ShaderTemplate& shader_template);
        ~OpenGLShader() override;

        void bind() override;
        void unbind() override;
        void* getStageHandle(ShaderStage stage) override;

    private:
        std::vector<char> loadBinaryFile(const std::string& path) override;
    };

}

#endif //OPENGLSHADER_H
