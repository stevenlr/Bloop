#include "maths/Matrix.h"

using namespace std;

template class Matrix<2>;
template class Matrix<3>;
template class Matrix<4>;

namespace {
	inline float det2x2(float *_, int a, int b, int c, int d)
	{
		return _[a] * _[d] - _[b] * _[c];
	}

	inline float det3x3(float *_, int a, int b, int c, int d, int e, int f, int g, int h, int i)
	{
		return _[a] * (_[e] * _[i] - _[f] * _[h])
				+ _[b] * (_[f] * _[g] - _[d] * _[i])
				+ _[c] * (_[d] * _[h] - _[e] * _[g]);
	}
}

template <>
float Matrix<2>::determinant()
{
	return det2x2(_data, 0, 1, 2, 3);
}

template <>
float Matrix<3>::determinant()
{
	return det3x3(_data, 0, 1, 2, 3, 4, 5, 6, 7, 8);
}

template <>
float Matrix<4>::determinant()
{
	return _data[0] * det3x3(_data, 5, 6, 7, 9, 10, 11, 13, 14, 15)
			- _data[1] * det3x3(_data, 4, 6, 7, 8, 10, 11, 12, 14, 15)
			+ _data[2] * det3x3(_data, 4, 5, 7, 8, 9, 11, 12, 13, 15)
			- _data[3] * det3x3(_data, 4, 5, 6, 8, 9, 10, 12, 13, 14);
}

template <>
void Matrix<2>::invert()
{
	float det = determinant();

	if (det == 0)
		throw domain_error("Matrix not invertible.");

	swap(_data[0], _data[3]);
	_data[1] *= -1;
	_data[2] *= -1;

	(*this) *= 1 / det;
}

template <>
void Matrix<3>::invert()
{
	float det = determinant();

	if (det == 0)
		throw domain_error("Matrix not invertible.");

	float old[9];

	copy_n(_data, 9, old);

	_data[0] =  det2x2(old, 4, 5, 7, 8);
	_data[3] = -det2x2(old, 3, 5, 6, 8);
	_data[6] =  det2x2(old, 3, 4, 6, 7);

	_data[1] = -det2x2(old, 1, 2, 7, 8);
	_data[4] =  det2x2(old, 0, 2, 6, 8);
	_data[7] = -det2x2(old, 0, 1, 6, 7);

	_data[2] =  det2x2(old, 1, 2, 4, 5);
	_data[5] = -det2x2(old, 0, 2, 3, 5);
	_data[8] =  det2x2(old, 0, 1, 3, 4);

	(*this) *= 1 / det;
}

template <>
void Matrix<4>::invert()
{
	float det = determinant();

	if (det == 0)
		throw domain_error("Matrix not invertible.");

	float old[16];

	copy_n(_data, 16, old);

	_data[0]  =  det3x3(old, 5, 6, 7, 9, 10, 11, 13, 14, 15);
	_data[4]  = -det3x3(old, 4, 6, 7, 8, 10, 11, 12, 14, 15);
	_data[8]  =  det3x3(old, 4, 5, 7, 8, 9, 11, 12, 13, 15);
	_data[12] = -det3x3(old, 4, 5, 6, 8, 9, 10, 12, 13, 14);

	_data[1]  = -det3x3(old, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	_data[5]  =  det3x3(old, 0, 2, 3, 8, 10, 11, 12, 14, 15);
	_data[9]  = -det3x3(old, 0, 1, 3, 8, 9, 11, 12, 13, 15);
	_data[13] =  det3x3(old, 0, 1, 2, 8, 9, 10, 12, 13, 14);

	_data[2]  =  det3x3(old, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	_data[6]  = -det3x3(old, 0, 2, 3, 4, 6, 7, 12, 14, 15);
	_data[10] =  det3x3(old, 0, 1, 3, 4, 5, 7, 12, 13, 15);
	_data[14] = -det3x3(old, 0, 1, 2, 4, 5, 6, 12, 13, 14);

	_data[3]  = -det3x3(old, 1, 2, 3, 5, 6, 7, 9, 10, 11);
	_data[7]  =  det3x3(old, 0, 2, 3, 4, 6, 7, 8, 10, 11);
	_data[11] = -det3x3(old, 0, 1, 3, 4, 5, 7, 8, 9, 11);
	_data[15] =  det3x3(old, 0, 1, 2, 4, 5, 6, 8, 9, 10);

	(*this) *= 1 / det;
}
