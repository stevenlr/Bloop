#ifndef GRAPHICS_OPENGL_ELEMENTINDEXARRAY_H
#define GRAPHICS_OPENGL_ELEMENTINDEXARRAY_H

#include <GL/glew.h>

#include "graphics/opengl/Buffer.h"

class ElementIndexArray {
public:
	enum Type : GLenum {
		UnsignedInt = GL_UNSIGNED_INT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		UnsignedByte = GL_UNSIGNED_BYTE
	};

	ElementIndexArray(Buffer *buffer, Type type = UnsignedInt);
	ElementIndexArray(const ElementIndexArray &eai);
	~ElementIndexArray();

	void bind() const;
	void unbind() const;
	Type getType() const;

private:
	Buffer *_buffer;
	Type _type;
};

#endif
