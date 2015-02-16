#include "graphics/opengl/VertexArray.h"

#include <stdexcept>

using namespace std;

GLuint VertexArray::_boundVertexArray = 0;
uint16_t VertexArray::_enabledAttribs = 0;

namespace {
	uint16_t attribMasks[] = {
		0x0001, 0x0002, 0x0004, 0x0008,
		0x0010, 0x0020, 0x0040, 0x0080,
		0x0100, 0x0200, 0x0400, 0x0800,
		0x1000, 0x2000, 0x4000, 0x8000
	};
};

VertexArray::VertexArray(DrawMode mode, GLint offset, GLint count) :
		_mode(mode), _offset(offset), _count(count), _attribs(0)
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

		enableAttribs();
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

void VertexArray::addAttrib(const VertexAttrib &attrib)
{
	if (_boundVertexArray != _id)
		glBindVertexArray(_id);

	attrib.apply();
	_attribs |= attribMasks[attrib.getIndex()];

	if (_boundVertexArray != _id) {
		glBindVertexArray(_boundVertexArray);
	} else {
		glEnableVertexAttribArray(attrib.getIndex());
		_enabledAttribs |= attribMasks[attrib.getIndex()];
	}
}

void VertexArray::enableAttribs() const
{
	uint16_t toDisable = (~_attribs) & _enabledAttribs;
	uint16_t toEnable = _attribs & (~_enabledAttribs);
	uint16_t mask = 1;

	for (int i = 0; i < 16; ++i) {
		if (toDisable & mask)
			glDisableVertexAttribArray(i);

		if (toEnable & mask)
			glEnableVertexAttribArray(i);

		mask <<= 1;
	}

	_enabledAttribs = _attribs;
}
