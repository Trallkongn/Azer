#pragma once
#include <memory>
#include <string>
#include "FileFormatRecognizer.h"

namespace Azer {

    class Asset;

    class SourceLoader {
    public:
        static SourceLoader& Instance();

        Ref<Asset> Load(const std::string& path);

    private:
        SourceLoader() = default;
        ~SourceLoader() = default;
        SourceLoader(const SourceLoader&) = delete;
        SourceLoader& operator=(const SourceLoader&) = delete;

        Ref<Asset> LoadTexture(const std::string& path);
        Ref<Asset> SourceLoader::LoadGLTF(const std::string& path);
        Ref<Asset> SourceLoader::LoadHDR(const std::string& path);
    };
}