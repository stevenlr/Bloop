#ifndef MATHS_MATRIX_H
#define MATHS_MATRIX_H

#include "maths/Vector.h"

template <int N>
class Matrix {
public:
	Matrix();
	Matrix(const Matrix<N> &m);

	Matrix<N> &operator=(const Matrix<N> &m);
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

private:
	float _data[N * N];
};

extern template class Matrix<2>;
extern template class Matrix<3>;
extern template class Matrix<4>;

typedef Matrix<2> Matrix2;
typedef Matrix<3> Matrix3;
typedef Matrix<4> Matrix4;

#endif
