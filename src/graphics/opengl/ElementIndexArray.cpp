#include "graphics/opengl/ElementIndexArray.h"

#include <stdexcept>

using namespace std;

ElementIndexArray::ElementIndexArray(Buffer *buffer, ElementIndexArray::Type type) :
		_type(type)
{
	if (!buffer)
		throw runtime_error("Assigning null buffer to element index array.");

	_buffer = buffer;
}

ElementIndexArray::ElementIndexArray(const ElementIndexArray &eia)
{
	if (this == &eia)
		return;

	_buffer = eia._buffer;
}

ElementIndexArray::~ElementIndexArray()
{
}

void ElementIndexArray::bind() const
{
	_buffer->bind(Buffer::ElementArray);
}

void ElementIndexArray::unbind() const
{
	_buffer->unbind();
}

ElementIndexArray::Type ElementIndexArray::getType() const
{
	return _type;
}
