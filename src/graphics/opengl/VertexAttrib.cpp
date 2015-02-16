#include "graphics/opengl/VertexAttrib.h"

#include <stdexcept>

using namespace std;

VertexAttrib::VertexAttrib(Buffer *buffer, GLuint index,
						   GLint size, Type type, bool normalized,
						   GLsizei stride, const void *offset,
						   GLuint divisor)
{
	if (index >= 16)
		throw runtime_error("Exceeding maximum vertex attribute index.");

	_buffer = buffer;
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

void VertexAttrib::apply() const
{
	_buffer->bind(Buffer::Array);

	if (_type == Float || _type == Double)
		glVertexAttribPointer(_index, _size, _type, _normalized, _stride, _offset);
	else
		glVertexAttribIPointer(_index, _size, _type, _stride, _offset);

	if (_divisor != 0)
		glVertexAttribDivisor(_index, _divisor);
	
	_buffer->unbind();
}

GLuint VertexAttrib::getIndex() const
{
	return _index;
}
