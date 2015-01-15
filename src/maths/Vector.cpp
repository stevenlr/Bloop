#include "maths/Vector.h"

#include <cstring>
#include <cmath>

using namespace std;

template struct Vector<2>;
template struct Vector<3>;

template <int S>
Vector<S>::Vector(const Vector<S> &v)
{
	memcpy(data, v.data, S * sizeof(float));
}

template <int S>
Vector<S>::Vector(initializer_list<float> values)
{
	if (values.size() != S)
		return;

	float *ptr = data;
	initializer_list<float>::iterator it = values.begin();

	while (it != values.end()) {
		*ptr++ = *it++;
	}
}

template <int S>
Vector<S> &Vector<S>::operator=(const Vector<S> &v)
{
	memcpy(data, v.data, S * sizeof(float));
	return *this;
}

template <int S>
float &Vector<S>::operator[](int i)
{
	if (i < 0 || i >= S) {
		throw exception("Invalid vector index.");
	}

	return data[i];
}

template <int S>
Vector<S> Vector<S>::operator+(const Vector<S> &v) const
{
	Vector<S> v2;

	for (int i = 0; i < S; ++i) {
		v2.data[i] = v.data[i] + data[i];
	}

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator+=(const Vector<S> &v)
{
	for (int i = 0; i < S; ++i) {
		data[i] += v.data[i];
	}

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator-(const Vector<S> &v) const
{
	Vector<S> v2;

	for (int i = 0; i < S; ++i) {
		v2.data[i] = data[i] - v.data[i];
	}

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator-=(const Vector<S> &v)
{
	for (int i = 0; i < S; ++i) {
		data[i] -= v.data[i];
	}

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator*(float f) const
{
	Vector<S> v2;

	for (int i = 0; i < S; ++i) {
		v2.data[i] = data[i] * f;
	}

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator*=(float f)
{
	for (int i = 0; i < S; ++i) {
		data[i] *= f;
	}

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator/(float f) const
{
	Vector<S> v2;

	for (int i = 0; i < S; ++i) {
		v2.data[i] = data[i] / f;
	}

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator/=(float f)
{
	for (int i = 0; i < S; ++i) {
		data[i] /= f;
	}

	return *this;
}

template <int S>
float Vector<S>::dot(const Vector<S> &v) const
{
	float result = 0;

	for (int i = 0; i < S; ++i) {
		result += v.data[i] * data[i];
	}

	return result;
}

template <int S>
float Vector<S>::length() const
{
	float result = 0;

	for (int i = 0; i < S; ++i) {
		result += data[i] * data[i];
	}

	return sqrt(result);
}

template <>
Vector<3> Vector<3>::cross(const Vector<3> &v) const
{
	Vector<3> v2;

	v2.data[0] = data[1] * v.data[2] - data[2] * v.data[1];
	v2.data[1] = data[2] * v.data[0] - data[0] * v.data[2];
	v2.data[2] = data[0] * v.data[1] - data[1] * v.data[0];

	return v2;
}

template <int S>
Vector<S> Vector<S>::cross(const Vector<S> &v) const
{
	throw exception("Unimplemented method.");
	return Vector<S>();
}
