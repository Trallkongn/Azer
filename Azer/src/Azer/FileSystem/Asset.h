#pragma once
#include <string>
#include <Azer/Core/UUID.hpp>

namespace Azer {

    enum class FileFormat;

    class Asset
    {
    public:
        virtual ~Asset() = default;

        virtual FileFormat GetFileFormat() const = 0;

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        UUID GetAssetID() const { return m_ID; }

    protected:
        std::string m_Name;
        UUID m_ID = UUID::GenerateV4();
    };
}

