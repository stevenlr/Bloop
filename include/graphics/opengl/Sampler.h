#ifndef GRAPHICS_OPENGL_SAMPLER_H
#define GRAPHICS_OPENGL_SAMPLER_H

#include <GL/glew.h>

#include "graphics/opengl/Texture.h"

class Sampler {
public:

	enum MagFilter : GLenum {
		MagLinear = GL_LINEAR,
		MagNearest = GL_NEAREST
	};

	enum MinFilter : GLenum {
		MinLinear = GL_LINEAR,
		MinNearest = GL_NEAREST,
		MinLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		MinLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
		MinNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		MinNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR
	};

	enum Wrap : GLenum {
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER
	};

	Sampler(MinFilter minFilter, MagFilter magFilter, Wrap wrap);
	~Sampler();

	Sampler(const Sampler &sampler) = delete;
	Sampler &operator=(const Sampler &sampler) = delete;

	void bind(int unit) const;
	void unbind(int unit) const;

private:
	GLuint _id;
	static GLuint _samplerBound[MAX_TEXTURE_UNITS];
};

#endif
