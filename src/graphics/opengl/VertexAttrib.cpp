#include "graphics/opengl/VertexAttrib.h"

VertexAttrib::VertexAttrib(const Buffer &buffer, GLuint index,
						   GLint size, Type type, bool normalized,
						   GLsizei stride, const void *offset,
						   GLuint divisor)
{
	_buffer = buffer.getId();
	_index = index;
	_size = size;
	_type = type;
	_normalized = normalized;
	_stride = stride;
	_offset = static_cast<const GLvoid *>(offset);
	_divisor = divisor;
}

VertexAttrib::VertexAttrib(const VertexAttrib &vao)
{
	_buffer = vao._buffer;
	_index = vao._index;
	_size = vao._size;
	_type = vao._type;
	_normalized = vao._normalized;
	_stride = vao._stride;
	_offset = vao._offset;
	_divisor = vao._divisor;
}

VertexAttrib::~VertexAttrib()
{
}

VertexAttrib &VertexAttrib::operator=(const VertexAttrib &vao)
{
	_buffer = vao._buffer;
	_index = vao._index;
	_size = vao._size;
	_type = vao._type;
	_normalized = vao._normalized;
	_stride = vao._stride;
	_offset = vao._offset;
	_divisor = vao._divisor;

	return *this;
}
