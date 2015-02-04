#include "maths/MatrixFactory.h"

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
