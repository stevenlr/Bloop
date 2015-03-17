#ifndef MATHS_VECTOR_H
#define MATHS_VECTOR_H

#include <initializer_list>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <stdexcept>

template <int S>
class Vector {
public:
	Vector()
	{
		std::fill_n(_data, S, 0.0f);
	}

	Vector(const Vector<S> &v)
	{
		std::copy_n(v._data, S, _data);
	}

	Vector(std::initializer_list<float> values)
	{
		std::copy_n(values.begin(), std::min(static_cast<int>(values.size()), S), _data);
	}

	Vector<S> &operator=(const Vector<S> &v)
	{
		if (&v == this)
			return *this;

		std::copy_n(v._data, S, _data);

		return *this;
	}

	inline float &operator[](int i)
	{
		if (i < 0 || i >= S) {
			throw std::out_of_range("Invalid vector index.");
		}

		return _data[i];
	}

	inline float operator[](int i) const
	{
		if (i < 0 || i >= S) {
			throw std::out_of_range("Invalid vector index.");
		}

		return _data[i];
	}

	inline const float *getData() const
	{
		return _data;
	}

	Vector<S> operator+(const Vector<S> &v) const
	{
		Vector<S> v2;
		const float *src1 = _data, *src2 = v._data;
		float *dst = v2._data;

		for (int i = 0; i < S; ++i) {
			*dst++ = *src1++ + *src2++;
		}

		return v2;
	}

	Vector<S> &operator+=(const Vector<S> &v)
	{
		const float *src = v._data;
		float *dst = _data;

		for (int i = 0; i < S; ++i) {
			*dst++ += *src++;
		}

		return *this;
	}

	Vector<S> operator-(const Vector<S> &v) const
	{
		Vector<S> v2;
		const float *src1 = _data, *src2 = v._data;
		float *dst = v2._data;

		for (int i = 0; i < S; ++i) {
			*dst++ = *src1++ - *src2++;
		}

		return v2;
	}

	Vector<S> &operator-=(const Vector<S> &v)
	{
		const float *src = v._data;
		float *dst = _data;

		for (int i = 0; i < S; ++i) {
			*dst++ -= *src++;
		}

		return *this;
	}

	Vector<S> operator*(float f) const
	{
		Vector<S> v2;
		const float *src = _data;
		float *dst = v2._data;

		for (int i = 0; i < S; ++i) {
			*dst++ = *src++ * f;
		}

		return v2;
	}

	Vector<S> &operator*=(float f)
	{
		float *dst = _data;

		for (int i = 0; i < S; ++i) {
			*dst++ *= f;
		}

		return *this;
	}

	Vector<S> operator/(float f) const
	{
		if (f == 0)
			throw std::domain_error("Division by 0.");

		return *this * (1 / f);
	}

	Vector<S> &operator/=(float f)
	{
		if (f == 0)
			throw std::domain_error("Division by 0.");

		*this *= 1 / f;

		return *this;
	}

	void normalize()
	{
		float len = length();

		if (len == 0)
			throw std::domain_error("Normalizing zero-length vector");

		*this /= len;
	}

	float dot(const Vector<S> &v) const
	{
		float sum = 0;
		const float *src1 = _data;
		const float *src2 = v._data;

		for (int i = 0; i < S; ++i) {
			sum += *src1++ * *src2++;
		}

		return sum;
	}

	float length() const
	{
		return std::sqrt(dot(*this));
	}

	Vector<S> cross(const Vector<S> &v) const
	{
		throw std::logic_error("Unimplemented method.");
	}

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
