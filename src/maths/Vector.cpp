#include "maths/Vector.h"

#include "maths/MultiplyConst.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <exception>

using namespace std;

template class Vector<2>;
template class Vector<3>;
template class Vector<4>;

template <int S>
Vector<S>::Vector(const Vector<S> &v)
{
	copy_n(v._data, S, _data);
}

template <int S>
Vector<S>::Vector(initializer_list<float> values)
{
	copy_n(values.begin(), min(static_cast<int>(values.size()), S), _data);
}

template <int S>
Vector<S> &Vector<S>::operator=(const Vector<S> &v)
{
	if (&v == this)
		return *this;

	copy_n(v._data, S, _data);

	return *this;
}

template <int S>
float &Vector<S>::operator[](int i)
{
	if (i < 0 || i >= S) {
		throw out_of_range("Invalid vector index.");
	}

	return _data[i];
}

template <int S>
float Vector<S>::operator[](int i) const
{
	if (i < 0 || i >= S) {
		throw out_of_range("Invalid vector index.");
	}

	return _data[i];
}

template <int S>
const float *Vector<S>::getData() const
{
	return _data;
}

template <int S>
Vector<S> Vector<S>::operator+(const Vector<S> &v) const
{
	Vector<S> v2;

	transform(_data, _data + S, v._data, v2._data, plus<float>());

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator+=(const Vector<S> &v)
{
	transform(_data, _data + S, v._data, _data, plus<float>());

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator-(const Vector<S> &v) const
{
	Vector<S> v2;

	transform(_data, _data + S, v._data, v2._data, minus<float>());

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator-=(const Vector<S> &v)
{
	transform(_data, _data + S, v._data, _data, minus<float>());

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator*(float f) const
{
	Vector<S> v2;

	transform(_data, _data + S, v2._data, MultiplyConst<float>(f));

	return v2;
}

template <int S>
Vector<S> &Vector<S>::operator*=(float f)
{
	for_each(_data, _data + S, MultiplyConst<float>(f));

	return *this;
}

template <int S>
Vector<S> Vector<S>::operator/(float f) const
{
	if (f == 0)
		throw domain_error("Division by 0.");

	return *this * (1 / f);
}

template <int S>
Vector<S> &Vector<S>::operator/=(float f)
{
	if (f == 0)
		throw domain_error("Division by 0.");

	*this *= 1 / f;

	return *this;
}

template <int S>
float Vector<S>::dot(const Vector<S> &v) const
{
	return inner_product(_data, _data + S, v._data, 0.0f);
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

	v2._data[0] = _data[1] * v._data[2] - _data[2] * v._data[1];
	v2._data[1] = _data[2] * v._data[0] - _data[0] * v._data[2];
	v2._data[2] = _data[0] * v._data[1] - _data[1] * v._data[0];

	return v2;
}

template <int S>
Vector<S> Vector<S>::cross(const Vector<S> &v) const
{
	throw logic_error("Unimplemented method.");
}
