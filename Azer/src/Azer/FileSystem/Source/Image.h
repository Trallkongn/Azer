#pragma once
#include "Azer/FileSystem/Asset.h"
#include <vector>

namespace Azer {

	struct ImageData
	{
		std::vector<unsigned char> buffer;
		const unsigned char* const_data = nullptr;
		float* HDRdata = nullptr;
		int width,height,channels;
	};

	class Image : public Asset
	{
	public:
		Image(const std::string& path, bool flip_vertically = false);
		~Image();

		const ImageData& GetData() const { return m_ImageData; }
		bool IsHDR() const { return HDR; }
	private:
		ImageData m_ImageData{};
		bool HDR = false;
	};
}