#include "maths/MatrixFactory.h"

#include <cmath>

#include "maths/MathsConsts.h"

using namespace std;

Matrix4 MatrixFactory::translation4x4(const Vector3 &offset)
{
	return translation4x4(offset[0], offset[1], offset[2]);
}

Matrix4 MatrixFactory::translation4x4(float x, float y, float z)
{
	Matrix4 m;

	m(0, 3) = x;
	m(1, 3) = y;
	m(2, 3) = z;

	return m;
}

Matrix4 MatrixFactory::scale4x4(const Vector3 &scale)
{
	return scale4x4(scale[0], scale[1], scale[2]);
}

Matrix4 MatrixFactory::scale4x4(float a)
{
	return scale4x4(a, a, a);
}

Matrix4 MatrixFactory::scale4x4(float x, float y, float z)
{
	Matrix4 m;

	m(0, 0) = x;
	m(1, 1) = y;
	m(2, 2) = z;

	return m;
}

Matrix4 MatrixFactory::rotationX4x4(float a)
{
	Matrix4 m;
	float c = cosf(a);
	float s = sinf(a);

	m(1, 1) = c;
	m(1, 2) = -s;
	m(2, 1) = s;
	m(2, 2) = c;

	return m;
}

Matrix4 MatrixFactory::rotationY4x4(float a)
{
	Matrix4 m;
	float c = cosf(a);
	float s = sinf(a);

	m(0, 0) = c;
	m(0, 2) = s;
	m(2, 0) = -s;
	m(2, 2) = c;

	return m;
}

Matrix4 MatrixFactory::rotationZ4x4(float a)
{
	Matrix4 m;
	float c = cosf(a);
	float s = sinf(a);

	m(0, 0) = c;
	m(0, 1) = -s;
	m(1, 0) = s;
	m(1, 1) = c;

	return m;
}

Matrix4 MatrixFactory::rotation4x4(const Vector3 &axis, float angle)
{
	Vector3 u = axis;
	Matrix4 m;
	float c = cosf(angle);
	float s = sinf(angle);
	float x = u[0], y = u[1], z = u[2];

	u.normalize();

	m(0, 0) = x * x + (1 - x * x) * c;
	m(0, 1) = x * y * (1 - c) - z * s;
	m(0, 2) = x * z * (1 - c) + y * s;

	m(1, 0) = x * y * (1 - c) + z * s;
	m(1, 1) = y * y + (1 - y * y) * c;
	m(1, 2) = y * z * (1 - c) - x * s;

	m(2, 0) = x * z * (1 - c) - y * s;
	m(2, 1) = y * z * (1 - c) + x * s;
	m(2, 2) = z * z + (1 - z * z) * c;

	return m;
}

Matrix4 MatrixFactory::rotation4x4(float ux, float uy, float uz, float angle)
{
	return rotation4x4(Vector3({ux, uy, uz}), angle);
}

Matrix4 MatrixFactory::rotation4x4(const Quaternion &q)
{
	return q.toRotationMatrix();
}

Matrix4 MatrixFactory::orthographicProjection4x4(float left, float right, float bottom, float top, float near, float far)
{
	float xs = 1 / (right - left);
	float ys = 1 / (top - bottom);
	float zs = 1 / (far - near);
	Matrix4 m;

	m(0, 0) = 2 * xs;
	m(1, 1) = 2 * ys;
	m(2, 2) = -2 * zs;

	m(0, 3) = -(right + left) * xs;
	m(1, 3) = -(top + bottom) * ys;
	m(2, 3) = -(far + near) * zs;

	return m;
}

Matrix4 MatrixFactory::perspectiveProjection4x4(float fov, float width, float height, float near, float far)
{
	Matrix4 m;
	float t = 1 / tanf(fov * PI / 360);

	m(0, 0) = t;
	m(1, 1) = t * width / height;
	m(2, 2) = (far + near) / (near - far);
	m(3, 3) = 0;

	m(2, 3) = 2 * far * near / (near - far);
	m(3, 2) = -1;

	return m;
}

Matrix4 MatrixFactory::lookAt4x4(const Vector3 &position, const Vector3 &target, const Vector3 &up)
{
	Vector3 view = target - position;
	view.normalize();

	Vector3 right = view.cross(up);
	right.normalize();

	Vector3 z = right.cross(view);
	Matrix4 m;

	m(0, 0) = right[0];
	m(0, 1) = right[1];
	m(0, 2) = right[2];

	m(1, 0) = z[0];
	m(1, 1) = z[1];
	m(1, 2) = z[2];

	m(2, 0) = -view[0];
	m(2, 1) = -view[1];
	m(2, 2) = -view[2];

	return m * translation4x4(-position[0], -position[1], -position[2]);
}
