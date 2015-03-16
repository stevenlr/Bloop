#ifndef MATHS_MATRIX_H
#define MATHS_MATRIX_H

#include <stdexcept>
#include <algorithm>
#include <stdexcept>
#include <numeric>

#include "maths/Vector.h"

template <int N>
class Matrix {
	friend class Matrix<N + 1>;

public:
	Matrix()
	{
		_data = new float[N * N];
		identity();
	}

	Matrix(const Matrix<N> &m)
	{
		if (this == &m)
			return;

		if (_data == nullptr)
			_data = new float[N * N];

		std::copy_n(m._data, N * N, _data);
	}

	Matrix(Matrix<N> &&m)
	{
		if (this == &m)
			return;

		if (_data != nullptr)
			delete[] _data;

		_data = m._data;
		m._data = nullptr;
	}

	~Matrix()
	{
		if (_data != nullptr)
			delete[] _data;
	}

	Matrix<N> &operator=(const Matrix<N> &m)
	{
		if (this == &m)
			return *this;

		if (_data == nullptr)
			_data = new float[N * N];

		std::copy_n(m._data, N * N, _data);

		return *this;
	}

	Matrix<N> &operator=(Matrix<N> &&m)
	{
		if (this == &m)
			return *this;

		if (_data != nullptr)
			delete[] _data;

		_data = m._data;
		m._data = nullptr;

		return *this;
	}

	inline float &operator()(int i, int j)
	{
		if (i < 0 || j < 0 || i >= N || j >= N) {
			throw std::out_of_range("Invalid matrix indices.");
		}

		return _data[i * N + j];
	}

	inline float operator()(int i, int j) const
	{
		if (i < 0 || j < 0 || i >= N || j >= N) {
			throw std::out_of_range("Invalid matrix indices.");
		}

		return _data[i * N + j];
	}

	inline const float *getData() const
	{
		return _data;
	}

	Matrix<N> operator+(const Matrix<N> &m) const
	{
		Matrix<N> m2;
		const float *src1 = _data, *src2 = m._data;
		float *dst = m2._data;

		for (int i = 0; i < N * N; ++i) {
			*dst++ = *src1++ + *src2++;
		}

		return m2;
	}

	Matrix<N> operator-(const Matrix<N> &m) const
	{
		Matrix<N> m2;
		const float *src1 = _data, *src2 = m._data;
		float *dst = m2._data;

		for (int i = 0; i < N * N; ++i) {
			*dst++ = *src1++ - *src2++;
		}

		return m2;
	}

	Matrix<N> operator*(float f) const
	{
		Matrix<N> m2;
		const float *src = _data;
		float *dst = m2._data;

		for (int i = 0; i < N * N; ++i) {
			*dst++ = *src++ * f;
		}

		return m2;
	}

	Matrix<N> operator/(float f) const
	{
		Matrix<N> m2;

		if (f == 0)
			throw std::domain_error("Division by 0.");

		return *this * (1 / f);
	}

	Matrix<N> operator*(const Matrix<N> &m) const
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

	Vector<N> operator*(const Vector<N> &v) const
	{
		Vector<N> v2;
		const float *dataptr = _data;

		for (int i = 0; i < N; ++i) {
			v2[i] = std::inner_product(dataptr, dataptr + N, v.getData(), 0.0f);
			dataptr += N;
		}

		return v2;
	}

	Matrix<N> &operator+=(const Matrix<N> &m)
	{
		const float *src = m._data;
		float *dst = _data;

		for (int i = 0; i < N * N; ++i) {
			*dst++ += *src++;
		}

		return *this;
	}

	Matrix<N> &operator-=(const Matrix<N> &m)
	{
		const float *src = m._data;
		float *dst = _data;

		for (int i = 0; i < N * N; ++i) {
			*dst++ -= *src++;
		}

		return *this;
	}

	Matrix<N> &operator*=(float f)
	{
		float *dst = _data;

		for (int i = 0; i < N * N; ++i) {
			*dst++ *= f;
		}

		return *this;
	}

	Matrix<N> &operator/=(float f)
	{
		if (f == 0)
			throw std::domain_error("Division by 0.");

		*this *= 1 / f;

		return *this;
	}

	Matrix<N> &operator*=(const Matrix<N> &m)
	{
		return *this = *this * m;
	}

	void identity()
	{
		std::fill_n(_data, N * N, 0.0f);

		for (int i = 0; i < N * N; i += N + 1) {
			_data[i] = 1;
		}
	}

	void transpose()
	{
		for (int i = 1; i < N; ++i) {
			for (int j = 0; j < i; ++j) {
				std::swap((*this)(i, j), (*this)(j, i));
			}
		}
	}

	float determinant()
	{
		throw std::logic_error("Unimplemented method.");
	}

	void invert()
	{
		throw std::logic_error("Unimplemented method.");
	}

private:
	float *_data = nullptr;
};

template <>
float Matrix<2>::determinant();

template <>
float Matrix<3>::determinant();

template <>
float Matrix<4>::determinant();

template <>
void Matrix<2>::invert();

template <>
void Matrix<3>::invert();

template <>
void Matrix<4>::invert();

extern template class Matrix<2>;
extern template class Matrix<3>;
extern template class Matrix<4>;

typedef Matrix<2> Matrix2;
typedef Matrix<3> Matrix3;
typedef Matrix<4> Matrix4;

#endif
