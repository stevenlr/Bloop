#include "graphics/PngLoader.h"

#include <stdexcept>
#include <fstream>
#include <iostream>

#include <png.h>

#include "Logger.h"

using namespace std;

#define PNGSIGNATURESIZE 8

// Inspired from http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/

namespace {
	bool isPngSignatureValid(ifstream &file)
	{
		png_byte signature[PNGSIGNATURESIZE];

		file.read(reinterpret_cast<char *>(signature), PNGSIGNATURESIZE);

		if (!file.good()) {
			return false;
		}

		return png_sig_cmp(signature, 0, PNGSIGNATURESIZE) == 0;
	}

	void readPngDataIfstream(png_structp png, png_bytep data, png_size_t length)
	{
		reinterpret_cast<ifstream *>(png_get_io_ptr(png))->read((char *) data, length);
	}

	const void *loadPng(const std::string &filename, Texture::InternalFormat &internalFormat, Texture::Format &format, int &width, int &height)
	{
		ifstream file(filename, ios_base::in | ios_base::binary);

		if (!file.is_open()) {
			LOGERROR << "Error when opening image " << filename << endl;
			throw runtime_error("Error when opening png image. See logs.");
		}

		if (!isPngSignatureValid(file)) {
			LOGERROR << "Couldn't validate signature for image " << filename << endl;
			throw runtime_error("Couldn't validate signature for png image. See logs.");
		}

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

		if (!png) 
			throw runtime_error("Error when create png read structure.");
	
		png_infop pngInfo = png_create_info_struct(png);

		if (!pngInfo) {
			png_destroy_read_struct(&png, nullptr, nullptr);
			throw runtime_error("Error when creating png info structure.");
		}

		png_bytep *rows = nullptr;
		char *data = nullptr;

		if (setjmp(png_jmpbuf(png))) {
			png_destroy_read_struct(&png, &pngInfo, nullptr);

			if (rows) delete[] rows;
			if (data) delete[] data;

			LOGERROR << "Error when reading png file" << filename << endl;
			throw runtime_error("Error when reading png file. See logs.");
		}

		png_set_read_fn(png, reinterpret_cast<png_voidp>(&file), readPngDataIfstream);
		png_set_sig_bytes(png, PNGSIGNATURESIZE);
		png_read_info(png, pngInfo);

		width = static_cast<int>(png_get_image_width(png, pngInfo));
		height = static_cast<int>(png_get_image_height(png, pngInfo));

		png_uint_32 bitDepth = png_get_bit_depth(png, pngInfo);
		png_uint_32 channels = png_get_channels(png, pngInfo);
		png_uint_32 colorType = png_get_color_type(png, pngInfo);

		switch (colorType) {
			case PNG_COLOR_TYPE_PALETTE:
				png_set_palette_to_rgb(png);
				channels = 3;           
				break;
			case PNG_COLOR_TYPE_GRAY:
				if (bitDepth < 8) {
					png_set_expand_gray_1_2_4_to_8(png);
					bitDepth = 8;
				}
				break;
		}

		if (png_get_valid(png, pngInfo, PNG_INFO_tRNS)) {
			png_set_tRNS_to_alpha(png);
			channels += 1;
		}

		rows = new png_bytep[height];
		data = new char[width * height * bitDepth * channels / 8];
		const unsigned int stride = width * bitDepth * channels / 8;

		for (int i = 0; i < height; ++i) {
			png_uint_32 offset = (height - i - 1) * stride;
			rows[i] = reinterpret_cast<png_bytep>(data + offset);
		}

		png_read_image(png, rows);

		delete[] rows;
		png_destroy_read_struct(&png, &pngInfo, nullptr);
		file.close();

		if (bitDepth != 8) {
			delete[] data;
			LOGERROR << "Invalid image format for image " << filename << endl;
			throw runtime_error("Invalid image format. See logs.");
		}

		switch (colorType) {
			case PNG_COLOR_TYPE_GRAY:
				format = Texture::Red;
				internalFormat = Texture::R8;
				break;
			case PNG_COLOR_TYPE_GRAY_ALPHA:
				format = Texture::RG;
				internalFormat = Texture::RG8;
				break;
			case PNG_COLOR_TYPE_RGB:
				format = Texture::RGB;
				internalFormat = Texture::RGB8;
				break;
			case PNG_COLOR_TYPE_RGB_ALPHA:
				format = Texture::RGBA;
				internalFormat = Texture::RGBA8;
				break;
		}

		return static_cast<const void *>(data);
	}
}

Texture *loadPngTexture(const std::string &filename, bool mipmaps)
{
	Texture *texture = nullptr;
	Texture::Format format;
	Texture::InternalFormat internalFormat;
	int width, height;

	const void *data = loadPng(filename, internalFormat, format, width, height);

	texture = new Texture(width, height, internalFormat, format, Texture::UnsignedByte, data);

	if (mipmaps)
		texture->generateMipmaps();

	delete[] data;

	return texture;
}

namespace {
	Cubemap::Side cubemapSides[6] = {
		Cubemap::PositiveX, Cubemap::NegativeX,
		Cubemap::PositiveY, Cubemap::NegativeY,
		Cubemap::PositiveZ, Cubemap::NegativeZ
	};
}

Cubemap *loadPngCubemap(std::initializer_list<std::string> filenames, bool mipmaps)
{
	Cubemap *cubemap = new Cubemap();
	Texture::Format format;
	Texture::InternalFormat internalFormat;
	int width, height;

	if (filenames.size() != 6)
		throw runtime_error("Invalid number of files in cubemap.");

	auto filenamesIterator = filenames.begin();

	for (int i = 0; i < 6; ++i) {
		const void *data = loadPng(*filenamesIterator++, internalFormat, format, width, height);

		cubemap->addTexture(cubemapSides[i], width, height, internalFormat, format, data);

		delete[] data;
	}

	if (mipmaps)
		cubemap->generateMipmaps();

	return cubemap;
}
