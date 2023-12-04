//
// Created by michal-swiatek on 30.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "rendering/Shader.h"

#include <fstream>

#include "core/Application.h"
#include "utility/Filesystem.h"

namespace nebula::rendering {

    Shader::Shader(std::string name, ShaderTemplate shader_template) :
            m_name(std::move(name)), m_template(std::move(shader_template))
    {}

    ShaderStage Shader::getShaderStageFromFilename(const std::string& filename)
    {
        if (filename.contains(".vert"))
            return ShaderStage::cVertex;
        if (filename.contains(".frag"))
            return ShaderStage::cFragment;

        throw std::runtime_error(std::format("File extension not supported: {}", filename));
    }

    std::vector<char> Shader::loadFile(const std::string& path)
    {
        if (checkIfFileIsBinary(path))
            return loadBinaryFile(path);
        return loadSourceFile(path);
    }

    std::vector<char> Shader::loadBinaryFile(const std::string& path)
    {
        return readFile(path, true);
    }

    std::vector<char> Shader::loadSourceFile(const std::string& path)
    {
        return readFile(path, false);
    }

    std::string Shader::shaderStageToString(const ShaderStage stage)
    {
        switch (stage)
        {
            case ShaderStage::cVertex:          return "VERTEX";
            case ShaderStage::cFragment:        return "FRAGMENT";
            case ShaderStage::cShaderProgram:   return "PROGRAM";
        }

        throw std::runtime_error(std::format("Unknown shader stage: {}", static_cast<int>(stage)));
    }

    bool Shader::checkIfFileIsBinary(const std::string& filename)
    {
        return filename.ends_with(".bin") || filename.ends_with(".spv");
    }

    std::vector<char> Shader::readFile(const std::string& path, const bool binary)
    {
        const filesystem::Path shader_directory = Application::getResourcesPath() / "shaders";

        std::string shader_path = path;
        if (!shader_path.starts_with(shader_directory.string()))
            shader_path = (shader_directory / shader_path).string();

        std::ifstream file(shader_path, std::ios::ate | (binary ? std::ios::binary : 0));
        if (!file.is_open())
            throw std::runtime_error(std::format("Unable to open file at {}", path));

        const auto file_size = file.tellg();
        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), file_size);

        return buffer;
    }

}
