#pragma once
#include <string>
#include <Azer/Core/UUID.hpp>
#include "FileFormat.h"

namespace Azer {

    class Asset
    {
    public:
        Asset()
          : m_Format(FileFormat::Unknown)
        {
            id = UUID::GenerateV4();
            m_Name = id.ToString();
        }
        virtual ~Asset() {};

        FileFormat GetFileFormat() const { return m_Format; };
        void SetAssetName(const std::string& name) { m_Name = name; }
        const std::string& GetAssetName() const { return m_Name; }
        const std::string& GetAssetID() const { return id.ToString(); }
    protected:
        UUID id;
        std::string m_Name;
        FileFormat m_Format;
        
    };
}

