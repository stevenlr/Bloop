#include "graphics/opengl/Cubemap.h"

#include <stdexcept>

using namespace std;

Cubemap::Cubemap()
{
	glGenTextures(1, &_id);

	if (!_id)
		throw runtime_error("Error when generating cubemap.");
}

Cubemap::~Cubemap()
{
	if (_id)
		glDeleteTextures(1, &_id);
}

void Cubemap::addTexture(Side side, int width, int height, Texture::InternalFormat internalFormat, Texture::Format format, const void *data)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
	glTexImage2D(side, 0, internalFormat, width, height, 0, format, Texture::UnsignedByte, data);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::bind(int unit)
{
	if (unit < 0 || unit > MAX_TEXTURE_UNITS)
		throw runtime_error("Cube map unit out of bounds.");

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
}

void Cubemap::unbind(int unit)
{
	if (unit < 0 || unit > MAX_TEXTURE_UNITS)
		throw runtime_error("Cube map unit out of bounds.");

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::generateMipmaps()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
