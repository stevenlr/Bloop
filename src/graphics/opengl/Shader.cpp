#include "graphics/opengl/Shader.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "Logger.h"

using namespace std;

Shader::Shader(const string &filename, Shader::Type type) :
		_type(type)
{
	_id = glCreateShader(type);

	if (_id == 0)
		throw runtime_error("Error when creating shader.");
	
	loadSource(filename);
	glCompileShader(_id);

	GLint success;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		GLint length;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);

		GLchar *log = new GLchar[length];

		glGetShaderInfoLog(_id, length, nullptr, log);
		LOGERROR << "Error when compiling shader " << filename << ":" << endl << log << endl;
		delete[] log;
		
		throw runtime_error("Error when compiling shader. See logs.");
	}
}

Shader::~Shader()
{
	if (_id != 0)
		glDeleteShader(_id);
}

void Shader::loadSource(const string &filename)
{
	ifstream file(filename, ios_base::binary | ios_base::in);

	if (!file.is_open()) {
		LOGERROR << "Couldn't open shader file " << filename << endl;
		throw runtime_error("Error when opening shader file. See logs.");
	}

	file.seekg(0, ios_base::end);
	int size = static_cast<int>(file.tellg());
	file.seekg(0, ios_base::beg);

	GLchar *data = new GLchar[size];
	file.read(data, size);
	file.close();

	glShaderSource(_id, 1, (const char**) &data, &size);

	delete[] data;
}

GLuint Shader::getId() const
{
	return _id;
}
