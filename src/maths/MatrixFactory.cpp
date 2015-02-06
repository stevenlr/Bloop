#include "maths/MatrixFactory.h"

#include <cmath>

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
