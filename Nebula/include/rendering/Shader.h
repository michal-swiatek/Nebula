//
// Created by michal-swiatek on 30.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <variant>

#include "core/Core.h"
#include "core/Types.h"

namespace nebula::rendering {

    enum class ShaderStage
    {
        cVertex,
        cFragment
    };

    struct NEBULA_API VertexShader
    {
        std::string vertex_stage;
        std::string fragment_stage;
    };

    using ShaderTemplate = std::variant<VertexShader>;

    class NEBULA_API Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        [[nodiscard]] const std::string& getName() const { return m_name; }
        [[nodiscard]] const ShaderTemplate& getTemplate() const { return m_template; }

        static bool checkIfFileIsBinary(const std::string& filename);
        static ShaderStage getShaderStageFromFilename(const std::string& filename);
        static std::vector<char> readFile(const std::string& path, bool binary);

        //  Defined in DetectPlatform.cpp
        static Reference<Shader> create(const std::string& name, const ShaderTemplate& shader_template);

    protected:
        explicit Shader(std::string name, ShaderTemplate shader_template);

        std::vector<char> loadFile(const std::string& path);
        virtual std::vector<char> loadBinaryFile(const std::string& path);
        virtual std::vector<char> loadSourceFile(const std::string& path);

    private:
        std::string m_name;
        ShaderTemplate m_template;
    };

}

#endif //SHADER_H
