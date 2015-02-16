#ifndef GRAPHICS_OPENGL_BUFFER_H
#define GRAPHICS_OPENGL_BUFFER_H

#include <GL/glew.h>

class Buffer {
public:
	enum Target : GLenum {
		Array = GL_ARRAY_BUFFER,
		CopyRead = GL_COPY_READ_BUFFER,
		CopyWrite = GL_COPY_WRITE_BUFFER,
		ElementArray = GL_ELEMENT_ARRAY_BUFFER,
		PixelPack = GL_PIXEL_PACK_BUFFER,
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
		Texture = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER, 
		Uniform = GL_UNIFORM_BUFFER
	};

	enum Usage : GLenum {
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ, 
		DynamicCopy = GL_DYNAMIC_COPY
	};

	Buffer(Target target, Usage usage);
	~Buffer();

	Buffer(const Buffer &b) = delete;
	Buffer &operator=(const Buffer &b) = delete;

	void data(GLsizeiptr size, const void *data);
	void subData(GLintptr offset, GLsizeiptr size, const void *data);

	void bind();
	void bind(Target target);
	void unbind();

	GLuint getId() const;

private:
	Target _target;
	Usage _usage;
	GLuint _id;

	bool _bound;
	Target _boundTo;
};

#endif
