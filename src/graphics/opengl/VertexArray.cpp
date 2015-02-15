#include "graphics/opengl/VertexArray.h"

#include <stdexcept>

using namespace std;

GLuint VertexArray::_boundVertexArray = 0;

bool VertexArray::_enabledAttribs[16] = {false, false, false, false,
										 false, false, false, false,
										 false, false, false, false,
										 false, false, false, false};

VertexArray::VertexArray() :
		_mode(Triangles), _offset(0), _count(0)
{
	glGenVertexArrays(1, &_id);

	if (_id == 0)
		throw runtime_error("Error when creating vertex array.");
}

VertexArray::~VertexArray()
{
	if (_id != 0)
		glDeleteVertexArrays(1, &_id);
}

void VertexArray::bind() const
{
	if (_boundVertexArray != _id) {
		glBindVertexArray(_id);
		_boundVertexArray = _id;
	}
}

void VertexArray::unbind() const
{
	if (_boundVertexArray == _id)
		glBindVertexArray(0);
	else
		throw runtime_error("Unbounding vertex array from other vertex array.");
}

void VertexArray::setMode(DrawMode mode)
{
	_mode = mode;
}

void VertexArray::setOffset(GLint offset)
{
	_offset = offset;
}

void VertexArray::setCount(GLint count)
{
	_count = count;
}

void VertexArray::drawArrays() const
{
	if (_boundVertexArray != _id)
		throw runtime_error("Drawing unbound vertex array.");

	glDrawArrays(_mode, _offset, _count);
}
