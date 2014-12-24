#ifndef INCLUDE_GRAPHICS_OPENGL_SHADER_H_
#define INCLUDE_GRAPHICS_OPENGL_SHADER_H_

#include <GL/glew.h>
#include <string>

class Shader {
public:
	Shader(const char *vertFilename, const char *fragFilename);
	~Shader();

private:
	GLuint _createShader(const char *filename, GLenum type);

	GLuint _program;
};

#endif
