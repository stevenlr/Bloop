#ifndef MATHS_MATRIX_H
#define MATHS_MATRIX_H

#include "maths/Vector.h"

template <int N>
class Matrix {
	friend class Matrix<N + 1>;

public:
	Matrix();
	Matrix(const Matrix<N> &m);
	Matrix(Matrix<N> &&m);
	~Matrix();

	Matrix<N> &operator=(const Matrix<N> &m);
	Matrix<N> &operator=(Matrix<N> &&m);
	float &operator()(int i, int j);
	float operator()(int i, int j) const;

	Matrix<N> operator+(const Matrix<N> &m) const;
	Matrix<N> operator-(const Matrix<N> &m) const;
	Matrix<N> operator*(float f) const;
	Matrix<N> operator/(float f) const;
	Matrix<N> operator*(const Matrix<N> &m) const;
	Vector<N> operator*(const Vector<N> &v) const;

	Matrix<N> &operator+=(const Matrix<N> &m);
	Matrix<N> &operator-=(const Matrix<N> &m);
	Matrix<N> &operator*=(float f);
	Matrix<N> &operator/=(float f);
	Matrix<N> &operator*=(const Matrix<N> &m);

	void identity();
	void transpose();
	float determinant();
	void invert();

private:
	float *_data;
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
