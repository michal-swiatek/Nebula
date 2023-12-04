//
// Created by michal-swiatek on 04.12.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <exception>
#include <filesystem>

namespace nebula::filesystem {

    class Path : public std::filesystem::path
    {
        using path::path;

    public:
        Path(path&& std_path);
        Path& operator = (path&& std_path);
        [[nodiscard]] bool mkdir(bool parents = true, bool exist_ok = true) const;
    };

    Path getCurrentWorkingDirectory();
    void setCurrentWorkingDirectory(const Path& path);

    bool checkPath(const Path& path);
    bool checkFile(const Path& path);
    bool checkDirectory(const Path& path);

    bool createDirectory(const Path& path, bool parents = true, bool exist_ok = true);

    std::vector<char> readFile(const Path& path);
    std::vector<std::byte> readBinaryFile(const Path& path);

    void saveFile(const Path& path, const std::vector<char>& data, bool make_parents = true);
    void saveBinaryFile(const Path& path, const std::vector<std::byte>& data, bool make_parents = true);

    //
    //  Exceptions
    //

    class FilesystemError : public std::exception
    {
    public:
        explicit FilesystemError(const std::error_code& error_code);

        [[nodiscard]] const char* what() const noexcept override { return "Error encountered while performing Filesystem operations."; }
        [[nodiscard]] const std::error_code& getErrorCode() const noexcept { return m_error_code; }

    private:
        std::error_code m_error_code;
    };

    class FileError final : public FilesystemError
    {
    public:
        FileError(Path path, const std::error_code& error_code);

        [[nodiscard]] const char* what() const noexcept override { return m_message.c_str(); }
        [[nodiscard]] const Path& getPath() const noexcept { return m_path; }

    private:
        Path m_path;
        std::string m_message;
    };

    class DirectoryError final : public FilesystemError
    {
    public:
        DirectoryError(Path path, const std::error_code& error_code);

        [[nodiscard]] const char* what() const noexcept override { return m_message.c_str(); }
        [[nodiscard]] const Path& getPath() const noexcept { return m_path; }

    private:
        Path m_path;
        std::string m_message;
    };

}

#endif //FILESYSTEM_H
