#ifndef GRAPHICS_OPENGL_CUBEMAP_H
#define GRAPHICS_OPENGL_CUBEMAP_H

#include <GL/glew.h>

#include "graphics/opengl/Texture.h"

class Cubemap {
public:
	enum Side  : GLenum {
		PositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		NegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		PositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		NegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		PositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		NegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	Cubemap();
	~Cubemap();

	Cubemap(const Cubemap &cubemap) = delete;
	Cubemap &operator=(const Cubemap &cubemap) = delete;

	void addTexture(Side side, int width, int height, Texture::InternalFormat internalFormat, Texture::Format format, const void *data);

	void bind(int unit);
	void unbind(int unit);

	void generateMipmaps();

private:
	GLuint _id;
};

#endif
