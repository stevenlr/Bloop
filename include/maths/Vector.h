#ifndef MATHS_VECTOR_H_
#define MATHS_VECTOR_H_

#include <initializer_list>
#include <exception>

template <int S>
struct Vector {
	Vector() = default;
	Vector(const Vector<S> &v);
	Vector(std::initializer_list<float> values);

	Vector<S> &operator=(const Vector<S> &v);
	float &operator[](int i);

	Vector<S> operator+(const Vector<S> &v) const;
	Vector<S> operator-(const Vector<S> &v) const;
	Vector<S> operator*(float f) const;
	Vector<S> operator/(float f) const;

	Vector<S> &operator+=(const Vector<S> &v);
	Vector<S> &operator-=(const Vector<S> &v);
	Vector<S> &operator*=(float f);
	Vector<S> &operator/=(float f);

	float dot(const Vector<S> &v) const;
	float length() const;
	Vector<S> cross(const Vector<S> &v) const;

	float data[S];
};

extern template struct Vector<2>;
extern template struct Vector<3>;

#endif
