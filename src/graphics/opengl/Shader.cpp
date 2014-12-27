#include "graphics/opengl/Shader.h"

#include <iostream>
#include <fstream>

#include "Logger.h"

using namespace std;

Shader::Shader(const char *vertFilename, const char *fragFilename)
{
	GLuint vert, frag;

	_program = glCreateProgram();

	vert = _createShader(vertFilename, GL_VERTEX_SHADER);
	frag = _createShader(fragFilename, GL_FRAGMENT_SHADER);

	if (vert == 0 or frag == 0)
		return;

	glAttachShader(_program, vert);
	glAttachShader(_program, frag);

	link();
}

Shader::~Shader()
{
}

void Shader::bind() const
{
	glUseProgram(_program);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::bindAttribLocation(const GLint index, const char *name)
{
	glBindAttribLocation(_program, index, name);
}

void Shader::bindFragDataLocation(const GLint index, const char *name)
{
	glBindFragDataLocation(_program, index, name);
}

void Shader::link()
{
	glLinkProgram(_program);

	GLint success;
	glGetProgramiv(_program, GL_LINK_STATUS, &success);

	if (!success) {
		GLint length;
		glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);

		GLchar *log = new GLchar[length];
		glGetProgramInfoLog(_program, length, nullptr, log);
		LOGERROR << "Error when linking program " << _program << endl << log << endl;

		delete[] log;
	}
}

GLuint Shader::_createShader(const char *filename, GLenum type) const
{
	GLuint shader = glCreateShader(type);

	ifstream file(filename, ios_base::binary | ios_base::in);

	if (!file.is_open()) {
		LOGERROR << "Couldn't open shader file " << filename << endl;
		return 0;
	}

	file.seekg(0, ios_base::end);
	int size = file.tellg();
	file.seekg(0, ios_base::beg);

	GLchar *data = new GLchar[size];
	file.read(data, size);
	file.close();

	glShaderSource(shader, 1, &data, &size);
	glCompileShader(shader);
	delete[] data;

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		GLchar *log = new GLchar[length];
		glGetShaderInfoLog(shader, length, nullptr, log);
		LOGERROR << "Error when compiling shader " << filename << ":" << endl << log << endl;

		delete[] log;
		return 0;
	}

	return shader;
}
