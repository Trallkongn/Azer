#pragma once
#include <string>
#include "Azer/Core/Core.h"
#include <Azer/FileSystem/Asset.h>

namespace Azer {

	class Texture : public Asset
	{
	public:
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height, const unsigned char* data = nullptr);
		static Ref<Texture2D> Create(const std::string& path, bool isHDR = false);
	};

	class CubeMap
	{
	public:
		virtual ~CubeMap() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetRendererID() const = 0;

		static Ref<CubeMap> Create(uint32_t width, uint32_t height);
		static Ref<CubeMap> Create(int miplevels);
	};
}