#include "graphics/opengl/Buffer.h"

#include <stdexcept>

using namespace std;

Buffer::Buffer(Buffer::Target target, Buffer::Usage usage) :
		_target(target), _usage(usage), _bound(false), _boundTo(target)
{
	glGenBuffers(1, &_id);

	if (_id == 0)
		throw runtime_error("Error when creating buffer.");
}

Buffer::~Buffer()
{
	if (_bound)
		unbind();

	if (_id != 0)
		glDeleteBuffers(1, &_id);
}

void Buffer::data(GLsizeiptr size, const void *data)
{
	bool wasBound = _bound;

	if (!wasBound)
		bind();

	glBufferData(_boundTo, size, data, _usage);

	if (!wasBound)
		unbind();
}

void Buffer::subData(GLintptr offset, GLsizeiptr size, const void *data)
{
	bool wasBound = _bound;

	if (!wasBound)
		bind();

	glBufferSubData(_boundTo, offset, size, data);

	if (!wasBound)
		unbind();
}

void Buffer::bind()
{
	bind(_target);
}

void Buffer::bind(Target target)
{
	if (!_bound) {
		glBindBuffer(target, _id);	
	} else {
		if (target != _boundTo) {
			glBindBuffer(_boundTo, 0);
			glBindBuffer(target, _id);
		}
	}

	_bound = true;
	_boundTo = target;
}

void Buffer::unbind()
{
	glBindBuffer(_boundTo, 0);
	_bound = false;
}

GLuint Buffer::getId() const
{
	return _id;
}
