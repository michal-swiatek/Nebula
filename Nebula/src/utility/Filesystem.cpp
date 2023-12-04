//
// Created by michal-swiatek on 04.12.2023.
// Github: https://github.com/michal-swiatek
//

#include "utility/Filesystem.h"

using namespace std::filesystem;

namespace nebula::filesystem {

    Path::Path(path&& std_path) : path(std_path) {}
    Path& Path::operator=(path&& std_path) { *this = Path(std::move(std_path)); return *this; }

    bool Path::mkdir(const bool parents, const bool exist_ok) const
    {
        return createDirectory(*this, parents, exist_ok);
    }

    Path getCurrentWorkingDirectory()
    {
        return current_path();
    }

    void setCurrentWorkingDirectory(const Path& path)
    {
        current_path(path);
    }

    bool checkPath(const Path& path)
    {
        return exists(path);
    }

    bool checkFile(const Path& path)
    {
        return exists(path) && (is_regular_file(path) || is_character_file(path));
    }

    bool checkDirectory(const Path& path)
    {
        return exists(path) && is_directory(path);
    }

    bool createDirectory(const Path& path, const bool parents, const bool exist_ok)
    {
        bool result;
        std::error_code error_code;

        if (parents)
            result = create_directories(path, error_code);
        else
            result = create_directory(path, error_code);

        if (error_code)
            throw DirectoryError(path, error_code);
        if (!result && !exist_ok)
            throw DirectoryError(path, std::make_error_code(std::errc::file_exists));

        return result;
    }

    template <typename T>
    std::vector<T> readFileImpl(const Path& path)
    {
        auto flags = std::ios::ate | std::ios::in;
        if (std::is_same_v<T, std::byte>)
            flags |= std::ios::binary;

        std::ifstream file(path, flags);
        if (!file.is_open())
            throw FileError(path.string(), std::make_error_code(std::errc::file_exists));

        const auto file_size = file.tellg();
        std::vector<T> buffer(file_size);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), file_size);

        return buffer;
    }

    template <typename T>
    void saveFileImpl(const Path& path, const std::vector<T>& data, const bool make_parents)
    {
        auto flags = std::ios::out;
        if (std::is_same_v<T, std::byte>)
            flags |= std::ios::binary;

        if (make_parents)
            createDirectory(path.parent_path(), make_parents);

        std::ofstream file(path, flags);
        if (!file.is_open())
            throw FileError(path.string(), std::make_error_code(std::errc::file_exists));

        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    std::vector<char> readFile(const Path& path)
    {
        return readFileImpl<char>(path);
    }

    std::vector<std::byte> readBinaryFile(const Path& path)
    {
        return readFileImpl<std::byte>(path);
    }

    void saveFile(const Path& path, const std::vector<char>& data, const bool make_parents)
    {
        saveFileImpl(path, data, make_parents);
    }

    void saveBinaryFile(const Path& path, const std::vector<std::byte>& data, const bool make_parents)
    {
        saveFileImpl(path, data, make_parents);
    }

    //////////////////////////////////////////////////////////////////
    /////  Exceptions  ///////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////

    FilesystemError::FilesystemError(const std::error_code& error_code) : m_error_code(error_code) {}

    FileError::FileError(
        Path path,
        const std::error_code& error_code
    ) :
            FilesystemError(error_code),
            m_path(std::move(path)),
            m_message(std::format("Unable to open file at {}", m_path.string()))
    {}

    DirectoryError::DirectoryError(
        Path path,
        const std::error_code& error_code
    ) :
            FilesystemError(error_code),
            m_path(std::move(path)),
            m_message(std::format("Unable to find directory at {}", m_path.string()))
    {}

}
