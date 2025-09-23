#pragma once
#include <string>
#include <memory>
#include <vector>

namespace Azer {


    class FileSystem {
    public:
        // Get instance
        static FileSystem& Instance();

        // load file to memory
        std::string ReadTextFile(const std::string& path);
        std::vector<uint8_t> ReadBinaryFile(const std::string& path);

    private:
        FileSystem() = default;
        ~FileSystem() = default;
        FileSystem(const FileSystem&) = delete;
        FileSystem& operator=(const FileSystem&) = delete;
    };

}