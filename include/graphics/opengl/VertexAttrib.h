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

	VertexAttrib(Buffer *buffer, GLint size, Type type,
				 bool normalized = false, GLsizei stride = 0,
				 const void *offset = nullptr, GLuint divisor = 0);
	VertexAttrib(const VertexAttrib &vao);
	~VertexAttrib();

	VertexAttrib &operator=(const VertexAttrib &vao);

	void apply(GLuint index) const;

private:
	Buffer *_buffer;
	GLint _size;
	Type _type;
	bool _normalized;
	GLsizei _stride;
	const GLvoid *_offset;
	GLuint _divisor;
};

#endif
