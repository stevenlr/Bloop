#include "maths/Vector.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <exception>

using namespace std;

template struct Vector<2>;
template struct Vector<3>;
template struct Vector<4>;

template <int S>
Vector<S>::Vector(const Vector<S> &v)
{
	copy_n(v.data, S, data);
}

template <int S>
Vector<S>::Vector(initializer_list<float> values)
{
	copy_n(values.begin(), min(static_cast<int>(values.size()), S), data);
}

template <int S>
Vector<S> &Vector<S>::operator=(const Vector<S> &v)
{
	copy_n(v.data, S, data);
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

	transform(data, data + S, v.data, v2.data, plus<float>());

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator+=(const Vector<S> &v)
{
	transform(data, data + S, v.data, data, plus<float>());

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator-(const Vector<S> &v) const
{
	Vector<S> v2;

	transform(data, data + S, v.data, v2.data, minus<float>());

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator-=(const Vector<S> &v)
{
	transform(data, data + S, v.data, data, minus<float>());

	return *this;
}

template <typename T>
class MultiplyConst {
public:
	MultiplyConst(T f) : _f(f) {}
	T operator()(T f) { return f * _f; };

private:
	T _f;
};

template <int S>
Vector<S> Vector<S>::operator*(float f) const
{
	Vector<S> v2;

	transform(data, data + S, v2.data, MultiplyConst<float>(f));

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator*=(float f)
{
	for_each(data, data + S, MultiplyConst<float>(f));

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator/(float f) const
{
	return *this * (1 / f);
}

template <int S>
Vector<S> &Vector<S>::operator/=(float f)
{
	*this *= (1 / f);

	return *this;
}

template <int S>
float Vector<S>::dot(const Vector<S> &v) const
{
	return inner_product(data, data + S, v.data, 0.0f);
}

template <int S>
float Vector<S>::length() const
{
	return sqrt(dot(*this));
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
