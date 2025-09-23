#pragma once
#include <string>
#include "FileFormat.h"

namespace Azer {

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

