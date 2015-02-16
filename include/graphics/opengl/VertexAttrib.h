#ifndef GRAPHICS_OPENGL_VERTEXATTRIBS_H
#define GRAPHICS_OPENGL_VERTEXATTRIBS_H

#include <GL/glew.h>

#include "graphics/opengl/Buffer.h"

class VertexAttrib {
public:
	enum Type : GLenum {
		Byte = GL_BYTE,
		UnsignedByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		UnsignedInt = GL_UNSIGNED_INT,
		Float = GL_FLOAT,
		Double = GL_DOUBLE
	};

	VertexAttrib(const Buffer &buffer, GLuint index, GLint size, Type type,
				 bool normalized, GLsizei stride, const void *offset, GLuint divisor = 0);
	VertexAttrib(const VertexAttrib &vao);
	~VertexAttrib();

	VertexAttrib &operator=(const VertexAttrib &vao);

private:
	GLuint _buffer;
	GLuint _index;
	GLint _size;
	Type _type;
	bool _normalized;
	GLsizei _stride;
	const GLvoid *_offset;
	GLuint _divisor;
};

#endif
