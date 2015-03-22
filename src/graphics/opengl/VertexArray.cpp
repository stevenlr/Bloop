#include "graphics/opengl/VertexArray.h"

#include <stdexcept>

using namespace std;

GLuint VertexArray::_boundVertexArray = 0;

VertexArray::VertexArray(DrawMode mode, GLint count, GLint offset) :
		_mode(mode), _offset(offset), _count(count), _useEia(false)
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
	if (_boundVertexArray == _id) {
		glBindVertexArray(0);
		_boundVertexArray = 0;
	} else {
		throw runtime_error("Unbounding vertex array from other vertex array.");
	}
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

void VertexArray::drawElements() const
{
	if (_boundVertexArray != _id)
		throw runtime_error("Drawing unbound vertex array.");

	if (!_useEia)
		throw runtime_error("Called drawElement on vertex array without element index array bound.");

	glDrawElements(_mode, _count, _eiaType, nullptr);
}

void VertexArray::drawArraysInstanced(int count) const
{
	if (_boundVertexArray != _id)
		throw runtime_error("Drawing unbound vertex array.");

	glDrawArraysInstanced(_mode, _offset, _count, count);
}

void VertexArray::drawElementsInstanced(int count) const
{
	if (_boundVertexArray != _id)
		throw runtime_error("Drawing unbound vertex array.");

	if (!_useEia)
		throw runtime_error("Called drawElement on vertex array without element index array bound.");

	glDrawElementsInstanced(_mode, _count, _eiaType, nullptr, count);
}

void VertexArray::addAttrib(GLuint index, const VertexAttrib &attrib)
{
	if (index >= 16)
		throw runtime_error("Exceeding maximum vertex attribute index.");

	if (_boundVertexArray != _id)
		glBindVertexArray(_id);

	glEnableVertexAttribArray(index);
	attrib.apply(index);

	if (_boundVertexArray != _id)
		glBindVertexArray(_boundVertexArray);
}

void VertexArray::setElementIndexArray(const ElementIndexArray &eia)
{
	if (_boundVertexArray != _id)
		glBindVertexArray(_id);

	eia.bind();
	_useEia = true;
	_eiaType = eia.getType();

	if (_boundVertexArray != _id)
		glBindVertexArray(_boundVertexArray);
}
