#ifndef GRAPHICS_OPENGL_SHADERPROGRAM
#define GRAPHICS_OPENGL_SHADERPROGRAM

#include <GL/glew.h>
#include <string>
#include <map>

#include "graphics/opengl/Uniform.h"

class ShaderProgram {
	friend Uniform;

public:
	ShaderProgram(const std::string &vertFilename, const std::string &fragFilename);
	~ShaderProgram();

	ShaderProgram(const ShaderProgram &program) = delete;
	ShaderProgram &operator=(const ShaderProgram &program) = delete;

	void link();

	void bindAttribLocation(const std::string &name, GLuint location);
	void bindFragDataLocation(const std::string &name, GLuint location);

	void bind() const;
	void unbind() const;

	Uniform getUniform(const std::string &name);
	Uniform operator[](const std::string &name);

private:
	std::string _vFile;
	std::string _fFile;
	GLuint _id;
	bool _linked;
	std::map<std::string, Uniform> _uniforms;

	static GLuint _boundProgram;
};

#endif
