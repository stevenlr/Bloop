#include "graphics/opengl/Uniform.h"

#include <stdexcept>

#include "graphics/opengl/ShaderProgram.h"

using namespace std;

Uniform::Uniform(GLint location, GLuint program) :
		_location(location), _program(program)
{
}

Uniform::Uniform(const Uniform &u)
{
	_location = u._location;
	_program = u._program;
}

Uniform &Uniform::operator=(const Uniform &u)
{
	_location = u._location;
	_program = u._program;

	return *this;
}

Uniform::operator bool() const
{
	return _location != -1;
}

void Uniform::set1f(GLfloat f) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform1f(_location, f);
}

void Uniform::set2f(GLfloat f1, GLfloat f2) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform2f(_location, f1, f2);
}

void Uniform::set2f(const Vector2 &v) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform2fv(_location, 1, v.getData());
}

void Uniform::set3f(GLfloat f1, GLfloat f2, GLfloat f3) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform3f(_location, f1, f2, f3);
}

void Uniform::set3f(const Vector3 &v) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform3fv(_location, 1, v.getData());
}

void Uniform::set4f(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform4f(_location, f1, f2, f3, f4);
}

void Uniform::set4f(const Vector4 &v) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform4fv(_location, 1, v.getData());
}

void Uniform::set1i(GLint i) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniform1i(_location, i);
}

void Uniform::setMatrix2(const Matrix2 &m) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniformMatrix2fv(_location, 1, GL_TRUE, m.getData());
}

void Uniform::setMatrix3(const Matrix3 &m) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniformMatrix3fv(_location, 1, GL_TRUE, m.getData());
}

void Uniform::setMatrix4(const Matrix4 &m) const
{
	if (ShaderProgram::_boundProgram != _program)
		throw runtime_error("Setting uniform on unbounded program.");

	glUniformMatrix4fv(_location, 1, GL_TRUE, m.getData());
}
