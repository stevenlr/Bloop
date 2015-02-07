#ifndef GRAPHICS_OPENGL_SHADER_H_
#define GRAPHICS_OPENGL_SHADER_H_

#include <GL/glew.h>
#include <string>

#include "maths/Matrix.h"

class Shader {
public:
	Shader(const char *vertFilename, const char *fragFilename);
	~Shader();

	void bind() const;
	void unbind() const;

	void bindAttribLocation(const GLint index, const char *name);
	void bindFragDataLocation(const GLint index, const char *name);
	void link();

private:
	GLuint _createShader(const char *filename, const GLenum type) const;

	GLuint _program;
};

#endif
