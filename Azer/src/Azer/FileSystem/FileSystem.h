#pragma once
#include <string>
#include <memory>
#include <vector>

namespace Azer {


    class FileSystem {
    public:
        // 获取单例
        static FileSystem& Instance();

        // 加载文件到内存
        std::string ReadTextFile(const std::string& path);
        std::vector<uint8_t> ReadBinaryFile(const std::string& path);

    private:
        FileSystem() = default;
        ~FileSystem() = default;
        FileSystem(const FileSystem&) = delete;
        FileSystem& operator=(const FileSystem&) = delete;
    };

}