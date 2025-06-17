#pragma once
#include "Azer/Core.h"

#include <string>

namespace Azer {
	class AZER_API Texture
	{
	public:
		Texture(const std::string& FilePath);
		~Texture();

		void Bind(unsigned int slot = 0) const;
		void UnBind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
	private:
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	};
}
