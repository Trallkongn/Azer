#include "azpch.h"
#include "FileSystem.h"
#include <fstream>
#include <sstream>

namespace Azer {

    FileSystem& FileSystem::Instance() {
        static FileSystem instance;
        return instance;
    }

    std::string FileSystem::ReadTextFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return {};
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    std::vector<uint8_t> FileSystem::ReadBinaryFile(const std::string& path) {
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) return {};
        return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
    }
}