#include "graphics/opengl/Renderbuffer.h"

#include <stdexcept>

using namespace std;

Renderbuffer::Renderbuffer(int width, int height, InternalFormat format)
{
	glGenRenderbuffers(1, &_id);

	if (!_id)
		throw runtime_error("Error when creating renderbuffer.");

	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Renderbuffer::~Renderbuffer()
{
	if (_id)
		glDeleteRenderbuffers(1, &_id);
}

GLuint Renderbuffer::getId() const
{
	return _id;
}
