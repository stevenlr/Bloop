#include "graphics/opengl/Texture.h"

#include <stdexcept>

using namespace std;

Texture::Texture(int width, int height, InternalFormat internalFormat, Format format, Type type, const void *data)
{
	glGenTextures(1, &_id);

	if (!_id)
		throw runtime_error("Error when generating texture.");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	if (_id)
		glDeleteTextures(1, &_id);
}

void Texture::bind(int unit)
{
	if (unit < 0 || unit > MAX_TEXTURE_UNITS)
		throw runtime_error("Texture unit out of bounds.");

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind(int unit)
{
	if (unit < 0 || unit > MAX_TEXTURE_UNITS)
		throw runtime_error("Texture unit out of bounds.");

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getId() const
{
	return _id;
}

void Texture::generateMipmaps()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _id);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
