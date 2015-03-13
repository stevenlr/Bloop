#include "graphics/opengl/VertexAttrib.h"

#include <stdexcept>

using namespace std;

VertexAttrib::VertexAttrib(Buffer *buffer,
						   GLint size, Type type, bool normalized,
						   GLsizei stride, const void *offset,
						   GLuint divisor)
{
	if (!buffer)
		throw runtime_error("Assigning null buffer to vertex attrib.");

	_buffer = buffer;
	_size = size;
	_type = type;
	_normalized = normalized;
	_stride = stride;
	_offset = static_cast<const GLvoid *>(offset);
	_divisor = divisor;
}

VertexAttrib::VertexAttrib(const VertexAttrib &vao)
{
	if (this == &vao)
		return;

	_buffer = vao._buffer;
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
	_size = vao._size;
	_type = vao._type;
	_normalized = vao._normalized;
	_stride = vao._stride;
	_offset = vao._offset;
	_divisor = vao._divisor;

	return *this;
}

void VertexAttrib::apply(GLuint index) const
{
	_buffer->bind(Buffer::Array);

	if (_type == Float || _type == Double)
		glVertexAttribPointer(index, _size, _type, _normalized, _stride, _offset);
	else
		glVertexAttribIPointer(index, _size, _type, _stride, _offset);

	if (_divisor != 0)
		glVertexAttribDivisor(index, _divisor);
	
	_buffer->unbind();
}

