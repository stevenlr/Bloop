#ifndef MATHS_VECTOR_H_
#define MATHS_VECTOR_H_

#include <initializer_list>

template <int S>
class Vector {
public:
	Vector() = default;
	Vector(const Vector<S> &v);
	Vector(std::initializer_list<float> values);

	Vector<S> &operator=(const Vector<S> &v);
	inline float &operator[](int i);
	inline float operator[](int i) const;
	inline const float *getData() const;

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

private:
	float _data[S];
};

template <>
Vector<3> Vector<3>::cross(const Vector<3> &v) const;

extern template class Vector<2>;
extern template class Vector<3>;
extern template class Vector<4>;

typedef Vector<2> Vector2;
typedef Vector<3> Vector3;
typedef Vector<4> Vector4;

#endif
