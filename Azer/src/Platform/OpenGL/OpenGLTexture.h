#pragma once
#include "Azer/Renderer/Texture.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <tiny_gltf.h>

namespace Azer {

	class OpenGLTexture2D : public Texture2D 
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height, const unsigned char* data = nullptr);
		OpenGLTexture2D(const std::string& path, bool isHDR = false);
		virtual ~OpenGLTexture2D();

		FileFormat GetFileFormat() const override { return m_FileFormat; }

		uint32_t GetRendererID() const override { return m_RendererID; }

		uint32_t GetWidth() const override { return m_Width; }
		uint32_t GetHeight() const override { return m_Height; }
		void Bind(uint32_t slot = 0) const override;

		void SetData(void* data, uint32_t size) override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;

		FileFormat m_FileFormat;
	};

	class OpenGLCubeMap : public CubeMap
	{
	public:
		OpenGLCubeMap(uint32_t width, uint32_t height);
		OpenGLCubeMap(int miplevels);
		~OpenGLCubeMap();

		// Í¨¹ý CubeMap ¼Ì³Ð
		void Bind(uint32_t slot = 0) const override;

		inline uint32_t GetRendererID() const override { return m_RendererID; }
	private:
		uint32_t m_RendererID;
	};
}