#ifndef GRAPHICS_OPENGL_SHADERPROGRAM
#define GRAPHICS_OPENGL_SHADERPROGRAM

#include <GL/glew.h>
#include <string>

class ShaderProgram {
public:
	ShaderProgram(const std::string &vertFilename, const std::string &fragFilename);
	~ShaderProgram();

	ShaderProgram(const ShaderProgram &program) = delete;
	ShaderProgram &operator=(const ShaderProgram &program) = delete;

	void link();

	void bindAttribLocation(const std::string &name, GLuint location);
	void bindFragDataLocation(const std::string &name, GLuint location);

	void bind();
	void unbind();

private:
	std::string _vFile;
	std::string _fFile;
	GLuint _id;
	bool _linked;
};

#endif
