#ifndef GRAPHICS_OPENGL_FRAMEBUFFER_H
#define GRAPHICS_OPENGL_FRAMEBUFFER_H

#include <vector>
#include <GL/glew.h>

#include "graphics/opengl/Texture.h"
#include "graphics/opengl/Renderbuffer.h"

class Framebuffer {
public:
	enum Target : GLenum {
		DrawFramebuffer = GL_DRAW_FRAMEBUFFER,
		ReadFramebuffer = GL_READ_FRAMEBUFFER
	};

	enum Attachment : GLenum {
		None = GL_NONE,
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
	};

	Framebuffer();
	~Framebuffer();

	void bind(Target target);
	void unbind(Target target);

	void drawBuffers(const std::vector<Attachment> &list);

	void attachTexture(const Texture &texture, Attachment attachment);
	void attachRenderbuffer(const Renderbuffer &renderbuffer, Attachment attachment);

private:
	GLuint _id;
	Target _boundTo;

	static GLuint _boundDraw;
	static GLuint _boundRead;
};

#endif
