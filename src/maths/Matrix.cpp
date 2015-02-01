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
	float det = 0;
	float mult = 1;

	for (int i = 0; i < N; ++i) {
		Matrix<N - 1> subMatrix;
		float *dstptr = subMatrix._data;
		float *srcptr = _data;

		for (int j = 0; j < N; ++j, srcptr += N) {
			if (i == j)
				continue;

			copy_n(srcptr + 1, N - 1, dstptr);
			dstptr += N - 1;
		}

		det += mult * (*this)(i, 0) * subMatrix.determinant();
		mult *= -1;
	}

	return det;
}

template <>
float Matrix<2>::determinant()
{
	return _data[0] * _data[3] - _data[1] * _data[2];
}
