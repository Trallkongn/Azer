#pragma once
#include <string>

namespace Azer {

    enum class FileFormat
    {
        Unknown = -1,
        PNG, JPEG, GIF, WAV, OGG, GLTF, GLB, ZIP, HDR,
        CUSTOM
    };

    class FileFormatRecognizer 
    {
    public:
        static FileFormatRecognizer& Instance();

        FileFormat Recognize(const std::string& path);

    private:
        FileFormatRecognizer() = default;
        ~FileFormatRecognizer() = default;
        FileFormatRecognizer(const FileFormatRecognizer&) = delete;
        FileFormatRecognizer& operator=(const FileFormatRecognizer&) = delete;

        FileFormat RecognizeByExtension(const std::string& ext);
        FileFormat RecognizeByMagic(const std::string& path);
    };
}

