#include "azpch.h"
#include "SourceLoader.h"
#include "FileSystem.h"

#include <Azer/Renderer/Texture.h>
#include <Azer/GLTF/GLTF.h>

namespace Azer {

    SourceLoader& SourceLoader::Instance() {
        static SourceLoader instance;
        return instance;
    }

    Ref<Asset> SourceLoader::Load(const std::string& path) {
        auto fmt = FileFormatRecognizer::Instance().Recognize(path);

        switch (fmt) {
        case FileFormat::PNG:
            return LoadTexture(path);
        case FileFormat::JPEG:
            return LoadTexture(path);
        case FileFormat::GLTF:
            return LoadGLTF(path);
        case FileFormat::GLB:
            return LoadGLTF(path);
        case FileFormat::HDR:
            return LoadHDR(path);
        default:
            return nullptr;
        }
    }

    Ref<Asset> SourceLoader::LoadTexture(const std::string& path) {
        return Texture2D::Create(path);
    }

    Ref<Asset> SourceLoader::LoadGLTF(const std::string& path)
    {
        return CreateRef<GLTF>(path);
    }

    Ref<Asset> SourceLoader::LoadHDR(const std::string& path)
    {
        return Texture2D::Create(path, true);
    }

}

