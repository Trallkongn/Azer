#include "azpch.h"
#include "Texture.h"
#include "../../../vendor/stb_image/stb_image.h"
#include "glad/glad.h"

Azer::Texture::Texture(const std::string& FilePath)
	:m_RendererID(0),m_Width(0),m_Height(0),m_BPP(0),m_FilePath(FilePath),m_LocalBuffer(nullptr)
{
	stbi_set_flip_vertically_on_load(1); // flip vertical
	stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D,m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Azer::Texture::~Texture()
{
	glDeleteTextures(1,&m_RendererID);
}

void Azer::Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Azer::Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
