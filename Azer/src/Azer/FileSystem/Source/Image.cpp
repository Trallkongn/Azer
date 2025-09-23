#include "azpch.h"
#include "Image.h"

#include "stb_image.h"
#include "Azer/FileSystem/FileFormatRecognizer.h"

namespace Azer {
	
	Image::Image(const std::string& path, bool flip_vertically)
	{
		m_Format = FileFormatRecognizer::Instance().Recognize(path);

		if (m_Format == FileFormat::HDR)
		{
			HDR = true;

			if (flip_vertically) stbi_set_flip_vertically_on_load(1);

			m_ImageData.HDRdata = stbi_loadf(path.c_str(),
				&m_ImageData.width,
				&m_ImageData.height,
				&m_ImageData.channels, 0);
		}
		else {
			if (flip_vertically) stbi_set_flip_vertically_on_load(1);

			unsigned char* data = stbi_load(path.c_str(),
				&m_ImageData.width,
				&m_ImageData.height,
				&m_ImageData.channels, 0);
			m_ImageData.buffer.push_back(*data);
			stbi_image_free(data);
		}
	}

	Image::~Image()
	{
		stbi_image_free(m_ImageData.buffer.data());
		stbi_image_free(m_ImageData.HDRdata);
	}
}


