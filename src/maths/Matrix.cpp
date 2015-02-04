#include "maths/Matrix.h"

#include "maths/MultiplyConst.h"

#include <algorithm>
#include <stdexcept>
#include <functional>
#include <numeric>

using namespace std;

template class Matrix<2>;
template class Matrix<3>;
template class Matrix<4>;

template <int N>
Matrix<N>::Matrix()
{
	identity();
}

template <int N>
Matrix<N>::Matrix(const Matrix<N> &m)
{
	copy_n(m._data, N * N, _data);
}

template <int N>
Matrix<N> &Matrix<N>::operator=(const Matrix<N> &m)
{
	if (&m == this)
		return *this;

	copy_n(m._data, N * N, _data);

	return *this;
}

template <int N>
float &Matrix<N>::operator()(int i, int j)
{
	if (i < 0 || j < 0 || i >= N || j >= N) {
		throw out_of_range("Invalid matrix indices.");
	}

	return _data[i * N + j];
}

template <int N>
float Matrix<N>::operator()(int i, int j) const
{
	if (i < 0 || j < 0 || i >= N || j >= N) {
		throw out_of_range("Invalid matrix indices.");
	}

	return _data[i * N + j];
}

template <int N>
Matrix<N> Matrix<N>::operator+(const Matrix<N> &m) const
{
	Matrix<N> m2;

	transform(_data, _data + N, m._data, m2._data, plus<float>());

	return m2;
}

template <int N>
Matrix<N> Matrix<N>::operator-(const Matrix<N> &m) const
{
	Matrix<N> m2;

	transform(_data, _data + N, m._data, m2._data, minus<float>());

	return m2;
}

template <int N>
Matrix<N> Matrix<N>::operator*(float f) const
{
	Matrix<N> m2;

	transform(_data, _data + N, m2._data, MultiplyConst<float>(f));

	return m2;
}

template <int N>
Matrix<N> Matrix<N>::operator/(float f) const
{
	Matrix<N> m2;

	if (f == 0)
		throw domain_error("Division by 0.");

	return *this * (1 / f);
}

template <int N>
Matrix<N> Matrix<N>::operator*(const Matrix<N> &m) const
{
	Matrix<N> m2;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			float sum = 0;

			for (int k = 0; k < N; ++k) {
				sum += (*this)(i, k) * m(k, j);
			}

			m2(i, j) = sum;
		}
	}

	return m2;
}

template <int N>
Vector<N> Matrix<N>::operator*(const Vector<N> &v) const
{
	Vector<N> v2;
	const float *dataptr = _data;

	for (int i = 0; i < N; ++i) {
		v2[i] = inner_product(dataptr, dataptr + N, v.data, 0.0f);
		dataptr += N;
	}

	return v2;
}

template <int N>
Matrix<N> &Matrix<N>::operator+=(const Matrix<N> &m)
{
	transform(_data, _data + N * N, m._data, _data, plus<float>());

	return *this;
}

template <int N>
Matrix<N> &Matrix<N>::operator-=(const Matrix<N> &m)
{
	transform(_data, _data + N * N, m._data, _data, minus<float>());

	return *this;
}

template <int N>
Matrix<N> &Matrix<N>::operator*=(float f)
{
	for_each(_data, _data + N * N, MultiplyConst<float>(f));

	return *this;
}

template <int N>
Matrix<N> &Matrix<N>::operator/=(float f)
{
	if (f == 0)
		throw domain_error("Division by 0.");

	*this *= 1 / f;

	return *this;
}

template <int N>
Matrix<N> &Matrix<N>::operator*=(const Matrix<N> &m)
{
	return *this = *this * m;
}

template <int N>
void Matrix<N>::identity()
{
	fill_n(_data, N * N, 0.0f);

	for (int i = 0; i < N * N; i += N + 1) {
		_data[i] = 1;
	}
}

template <int N>
void Matrix<N>::transpose()
{
	for (int i = 1; i < N; ++i) {
		for (int j = 0; j < i; ++j) {
			swap((*this)(i, j), (*this)(j, i));
		}
	}
}

template <int N>
float Matrix<N>::determinant()
{
	throw logic_error("Unimplemented method.");
}

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
	_data[1] = -det2x2(old, 3, 5, 6, 8);
	_data[2] =  det2x2(old, 3, 4, 6, 7);
	_data[3] = -det2x2(old, 1, 2, 7, 8);
	_data[4] =  det2x2(old, 0, 2, 6, 8);
	_data[5] = -det2x2(old, 0, 1, 6, 7);
	_data[6] =  det2x2(old, 1, 2, 4, 5);
	_data[7] = -det2x2(old, 0, 2, 3, 5);
	_data[8] =  det2x2(old, 0, 1, 3, 4);

	transpose();
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
	_data[1]  = -det3x3(old, 4, 6, 7, 8, 10, 11, 12, 14, 15);
	_data[2]  =  det3x3(old, 4, 5, 7, 8, 9, 11, 12, 13, 15);
	_data[3]  = -det3x3(old, 4, 5, 6, 8, 9, 10, 12, 13, 14);

	_data[4]  = -det3x3(old, 1, 2, 3, 9, 10, 11, 13, 14, 15);
	_data[5]  =  det3x3(old, 0, 2, 3, 8, 10, 11, 12, 14, 15);
	_data[6]  = -det3x3(old, 0, 1, 3, 8, 9, 11, 12, 13, 15);
	_data[7]  =  det3x3(old, 0, 1, 2, 8, 9, 10, 12, 13, 14);

	_data[8]  =  det3x3(old, 1, 2, 3, 5, 6, 7, 13, 14, 15);
	_data[9]  = -det3x3(old, 0, 2, 3, 4, 6, 7, 12, 14, 15);
	_data[10] =  det3x3(old, 0, 1, 3, 4, 5, 7, 12, 13, 15);
	_data[11] = -det3x3(old, 0, 1, 2, 4, 5, 6, 12, 13, 14);

	_data[12] = -det3x3(old, 1, 2, 3, 5, 6, 7, 9, 10, 11);
	_data[13] =  det3x3(old, 0, 2, 3, 4, 6, 7, 8, 10, 11);
	_data[14] = -det3x3(old, 0, 1, 3, 4, 5, 7, 8, 9, 11);
	_data[15] =  det3x3(old, 0, 1, 2, 4, 5, 6, 8, 9, 10);

	transpose();
	(*this) *= 1 / det;
}
