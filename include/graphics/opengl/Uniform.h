#ifndef GRAPHICS_OPENGL_UNIFORM_H
#define GRAPHICS_OPENGL_UNIFORM_H

#include <GL/glew.h>

#include "maths/Vector.h"
#include "maths/Matrix.h"

class Uniform {
public:
	Uniform(GLint location, GLuint program);
	Uniform(const Uniform &u);

	Uniform &operator=(const Uniform &u);

	operator bool() const;

	void set1f(GLfloat f) const;

	void set2f(GLfloat f1, GLfloat f2) const;
	void set2f(const Vector2 &v) const;

	void set3f(GLfloat f1, GLfloat f2, GLfloat f3) const;
	void set3f(const Vector3 &v) const;

	void set4f(GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4) const;
	void set4f(const Vector4 &v) const;

	void set1i(GLint i) const;

	void setMatrix2(const Matrix2 &m) const;

	void setMatrix3(const Matrix3 &m) const;

	void setMatrix4(const Matrix4 &m) const;

private:
	GLint _location;
	GLuint _program;
};

#endif
