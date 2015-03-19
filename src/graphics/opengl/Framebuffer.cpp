#include "graphics/opengl/Framebuffer.h"

#include <stdexcept>

using namespace std;

GLuint Framebuffer::_boundDraw = 0;
GLuint Framebuffer::_boundRead = 0;

Framebuffer::Framebuffer() :
		_boundTo(ReadFramebuffer)
{
	glGenFramebuffers(1, &_id);

	if (!_id)
		throw runtime_error("Error when generating framebuffer.");
}

Framebuffer::~Framebuffer()
{
	if (_id)
		glDeleteFramebuffers(1, &_id);
}

void Framebuffer::bind(Target target)
{
	GLuint &bound = (target == ReadFramebuffer) ? _boundRead : _boundDraw;

	if (bound != _id) {
		glBindFramebuffer(target, _id);
		bound = _id;
		_boundTo = target;
	}
}

void Framebuffer::unbind(Target target)
{
	GLuint &bound = (target == ReadFramebuffer) ? _boundRead : _boundDraw;

	if (bound != _id)
		throw runtime_error("Unbounding framebuffer from different framebuffer.");
	

	glBindFramebuffer(target, 0);
	bound = 0;
}

void Framebuffer::attachTexture(const Texture &texture, Attachment attachment)
{
	GLuint bound = (_boundTo == ReadFramebuffer) ? _boundRead : _boundDraw;

	if (bound != _id)
		glBindFramebuffer(_boundTo, _id);

	glFramebufferTexture2D(_boundTo, attachment, GL_TEXTURE_2D, texture.getId(), 0);

	if (bound != _id)
		glBindFramebuffer(_boundTo, bound);
}
