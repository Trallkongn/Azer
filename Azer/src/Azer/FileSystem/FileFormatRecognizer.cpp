#include "azpch.h"

#include "FileFormatRecognizer.h"
#include "FileSystem.h"
#include <algorithm>


namespace Azer {

    FileFormatRecognizer& FileFormatRecognizer::Instance() {
        static FileFormatRecognizer instance;
        return instance;
    }

    FileFormat FileFormatRecognizer::Recognize(const std::string& path) {
        // Try extent
        auto extPos = path.find_last_of('.');
        if (extPos != std::string::npos) {
            std::string ext = path.substr(extPos + 1);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            FileFormat fmt = RecognizeByExtension(ext);
            if (fmt != FileFormat::Unknown) return fmt;
        }
        // Try Magic
        return RecognizeByMagic(path);
    }

    FileFormat FileFormatRecognizer::RecognizeByExtension(const std::string& ext) {
        if (ext == "png") return FileFormat::PNG;
        if (ext == "jpg" || ext == "jpeg") return FileFormat::JPEG;
        if (ext == "gltf") return FileFormat::GLTF;
        if (ext == "wav") return FileFormat::WAV;
        if (ext == "gif") return FileFormat::GIF;
        if (ext == "ogg") return FileFormat::OGG;
        if (ext == "glb") return FileFormat::GLB;
        if (ext == "zip") return FileFormat::ZIP;
        if (ext == "hdr") return FileFormat::HDR;
        return FileFormat::Unknown;
    }

    FileFormat FileFormatRecognizer::RecognizeByMagic(const std::string& path) {
        auto data = FileSystem::Instance().ReadBinaryFile(path);
        if (data.size() < 4) return FileFormat::Unknown;

        // PNG 魔数
        if (data[0] == 0x89 && data[1] == 'P' && data[2] == 'N' && data[3] == 'G') return FileFormat::PNG;
        // JPEG 魔数
        if (data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF) return FileFormat::JPEG;
        // 可继续添加其他魔数识别

        return FileFormat::Unknown;
    }
}