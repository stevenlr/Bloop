#ifndef MATHS_VECTOR_H_
#define MATHS_VECTOR_H_

#include <initializer_list>

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
extern template struct Vector<4>;

typedef Vector<2> Vector2;
typedef Vector<3> Vector3;
typedef Vector<4> Vector4;

#endif
