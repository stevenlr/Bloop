#include "graphics/opengl/ShaderProgram.h"

#include <stdexcept>

#include "Logger.h"
#include "graphics/opengl/Shader.h"

using namespace std;

GLuint ShaderProgram::_boundProgram = 0;

ShaderProgram::ShaderProgram(const string &vertFilename, const string &fragFilename) :
		_linked(false), _vFile(vertFilename), _fFile(fragFilename)
{
	_id = glCreateProgram();

	if (_id == 0)
		throw runtime_error("Error when creating program.");

	Shader vert(vertFilename, Shader::VertexShader);
	Shader frag(fragFilename, Shader::FragmentShader);

	glAttachShader(_id, vert.getId());
	glAttachShader(_id, frag.getId());
}

ShaderProgram::~ShaderProgram()
{
	if (_id != 0)
		glDeleteProgram(_id);
}

void ShaderProgram::link()
{
	_linked = true;
	glLinkProgram(_id);

	GLint success;
	glGetProgramiv(_id, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		GLint length;

		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

		GLchar *log = new GLchar[length];

		LOGERROR << "Error when linking program (" << _vFile << ", " << _fFile << ") : "
			<< endl << log << endl;
		delete[] log;

		throw runtime_error("Error when linking program. See logs.");
	}

	_uniforms.clear();
}

void ShaderProgram::bindAttribLocation(const std::string &name, GLuint location)
{
	_linked = false;
	glBindAttribLocation(_id, location, static_cast<const GLchar *>(name.c_str()));
}

void ShaderProgram::bindFragDataLocation(const std::string &name, GLuint location)
{
	_linked = false;
	glBindFragDataLocation(_id, location, static_cast<const GLchar *>(name.c_str()));
}

void ShaderProgram::bind() const
{
	if (!_linked)
		throw runtime_error("Binding an unlinked program.");

#ifndef NDEBUG
	GLint success;

	glValidateProgram(_id);
	glGetProgramiv(_id, GL_VALIDATE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint length;

		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);

		GLchar *log = new GLchar[length];

		glGetProgramInfoLog(_id, length, nullptr, log);
		LOGERROR << "Couldn't validate program (" << _vFile << ", " << _fFile << ") : "
			<< endl << log << endl;
		delete[] log;

		throw runtime_error("Error when validating program. See logs.");
	}
#endif

	if (_boundProgram != _id) {
		glUseProgram(_id);
		_boundProgram = _id;
	}
}

void ShaderProgram::unbind() const
{
	if (_boundProgram == _id) {
		glUseProgram(0);
		_boundProgram = 0;
	}
	else
		throw runtime_error("Unbounding program from other program.");
}

Uniform ShaderProgram::getUniform(const std::string &name)
{
	auto it = _uniforms.find(name);

	if (it != _uniforms.end()) {
		return it->second;
	}

	Uniform uniform(glGetUniformLocation(_id, name.c_str()), _id);

	_uniforms.insert(make_pair(name, uniform));

	return uniform;
}

Uniform ShaderProgram::operator[](const std::string &name)
{
	return getUniform(name);
}
