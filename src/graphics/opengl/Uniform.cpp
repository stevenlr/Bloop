#include "graphics/opengl/Uniform.h"

Uniform::Uniform(GLint location) :
		_location(location)
{
}

Uniform::Uniform(const Uniform &u)
{
	_location = u._location;
}

Uniform &Uniform::operator=(const Uniform &u)
{
	_location = u._location;

	return *this;
}

Uniform::operator bool() const
{
	return _location != -1;
}

void Uniform::set1f(GLfloat f) const
{
	glUniform1f(_location, f);
}

void Uniform::set2f(GLfloat f1, GLfloat f2) const
{
	glUniform2f(_location, f1, f2);
}

void Uniform::set2f(const Vector2 &v) const
{
	glUniform2fv(_location, 1, v.getData());
}

void Uniform::set3f(GLfloat f1, GLfloat f2, GLfloat f3) const
{
	glUniform3f(_location, f1, f2, f3);
}

void Uniform::set3f(const Vector3 &v) const
{
	glUniform3fv(_location, 1, v.getData());
}

void Uniform::set4f(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4) const
{
	glUniform4f(_location, f1, f2, f3, f4);
}

void Uniform::set4f(const Vector4 &v) const
{
	glUniform4fv(_location, 1, v.getData());
}

void Uniform::set1i(GLint i) const
{
	glUniform1i(_location, i);
}

void Uniform::setMatrix2(const Matrix2 &m) const
{
	glUniformMatrix2fv(_location, 1, GL_TRUE, m.getData());
}

void Uniform::setMatrix3(const Matrix3 &m) const
{
	glUniformMatrix3fv(_location, 1, GL_TRUE, m.getData());
}

void Uniform::setMatrix4(const Matrix4 &m) const
{
	glUniformMatrix4fv(_location, 1, GL_TRUE, m.getData());
}
