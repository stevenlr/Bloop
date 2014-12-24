#include <Logger.h>
#include "graphics/opengl/Shader.h"

#include <iostream>
#include <fstream>


using namespace std;

Shader::Shader(const char *vertFilename, const char *fragFilename) {
	_program = glCreateProgram();

	_createShader(vertFilename, GL_VERTEX_SHADER);
	_createShader(fragFilename, GL_FRAGMENT_SHADER);
}

Shader::~Shader() {
}

GLuint Shader::_createShader(const char *filename, GLenum type) {
	GLuint shader = glCreateShader(type);

	ifstream file(filename);

	if (!file.is_open()) {
		LOGERROR << "Couldn't open shader file " << filename << endl;
		return -1;
	}

	file.seekg(0, ios_base::end);
	int size = file.tellg();
	file.seekg(0, ios_base::beg);

	file.close();

	return 0;
}
