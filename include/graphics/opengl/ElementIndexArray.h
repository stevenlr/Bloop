#ifndef GRAPHICS_OPENGL_ELEMENTINDEXARRAY_H
#define GRAPHICS_OPENGL_ELEMENTINDEXARRAY_H

#include <GL/glew.h>

#include "graphics/opengl/Buffer.h"

class ElementIndexArray {
public:
	ElementIndexArray(Buffer *buffer);
	ElementIndexArray(const ElementIndexArray &eai);
	~ElementIndexArray();

	void bind() const;
	void unbind() const;

private:
	Buffer *_buffer;
};

#endif
