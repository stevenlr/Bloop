#ifndef GRAPHICS_OPENGL_TEXTURE_H
#define GRAPHICS_OPENGL_TEXTURE_H

#include <GL/glew.h>

#define MAX_TEXTURE_UNITS 48

class Texture {
public:
	enum InternalFormat : GLenum {
		RGBA32f = GL_RGBA32F,
		RGBAi = GL_RGBA32I,
		RGBA32ui = GL_RGBA32UI,
		RGBA16 = GL_RGBA16,
		RGBA16f = GL_RGBA16F,
		RGBA16i = GL_RGBA16I,
		RGBA16ui = GL_RGBA16UI,
		RGBA8 = GL_RGBA8,
		RGBA9ui = GL_RGBA8UI,

		R32f = GL_R32F,
		R32i = GL_R32I,
		R32ui = GL_R32UI,
		R16f = GL_R16F,
		R16i = GL_R16I,
		R16ui = GL_R16UI,
		R8 = GL_R8,
		R8i = GL_R8I,
		R8ui = GL_R8UI,

		RG32f = GL_RG32F,
		RG32i = GL_RG32I,
		RG32ui = GL_RG32UI,
		RG16f = GL_RG16F,
		RG16i = GL_RG16I,
		RG16ui = GL_RG16UI,
		RG8 = GL_RG8,
		RG8i = GL_RG8I,
		RG8ui = GL_RG8UI,

		RGB32f = GL_RGB32F,
		RGB32i = GL_RGB32I,
		RGB32ui = GL_RGB32UI,
		RGB16f = GL_RGB16F,
		RGB16i = GL_RGB16I,
		RGB16ui = GL_RGB16UI,
		RGB16 = GL_RGB16,
		RGB8 = GL_RGB8,
		RGB8i = GL_RGB8I,
		RGB8ui = GL_RGB8UI,

		DepthComponent32f = GL_DEPTH_COMPONENT32F,
		DepthComponent24 = GL_DEPTH_COMPONENT24,
		DepthComponent16 = GL_DEPTH_COMPONENT16,

		Depth32fStencil8 = GL_DEPTH32F_STENCIL8,
		Depth24Stencil8 = GL_DEPTH24_STENCIL8
	};

	enum Format : GLenum {
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL
	};

	enum Type : GLenum {
		UnsignedByte = GL_UNSIGNED_BYTE,
		Byte = GL_BYTE,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT, 
		UnsignedInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Float = GL_FLOAT
	};

	Texture(int width, int height, InternalFormat internalFormat = RGB8ui, Format format = RGB, Type type = UnsignedByte, const void *data = nullptr);
	~Texture();

	Texture(const Texture &texture) = delete;
	Texture &operator=(const Texture &texture) = delete;

	void bind(int unit);
	void unbind(int unit);

	GLuint getId() const;

	void generateMipmaps();

private:
	GLuint _id;
};

#endif
