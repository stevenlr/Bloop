#include "maths/Quaternion.h"

#include <cmath>
#include <stdexcept>

using namespace std;

Quaternion::Quaternion() :
		_x(0), _y(0), _z(0), _w(0)
{
}

Quaternion::Quaternion(const Quaternion &q) :
		_x(q._x), _y(q._y), _z(q._z), _w(q._w)
{
}

Quaternion::Quaternion(const Vector3 &v, bool point) :
		_x(v[0]), _y(v[1]), _z(v[2]), _w((point) ? 1.f : 0.f)
{
}

Quaternion::Quaternion(const Vector4 &v) :
		_x(v[0]), _y(v[1]), _z(v[2]), _w(v[3])
{
}

Quaternion::Quaternion(const Vector3 &axis, float angle)
{
	Vector3 u = axis;

	u.normalize();
	u *= sinf(angle / 2);

	_x = u[0];
	_y = u[1];
	_z = u[2];

	_w = cosf(angle / 2);
}

Quaternion::Quaternion(float ux, float uy, float uz, float angle) :
		Quaternion(Vector3({ux, uy, uz}), angle)
{
}

Quaternion &Quaternion::operator=(const Quaternion &q)
{
	if (this == &q)
		return *this;

	_x = q._x;
	_y = q._y;
	_z = q._z;
	_w = q._x;

	return *this;
}

Quaternion Quaternion::operator*(float a) const
{
	Quaternion q;

	q._x = _x * a;
	q._y = _y * a;
	q._z = _z * a;
	q._w = _w * a;

	return q;
}

Quaternion Quaternion::operator/(float a) const
{
	Quaternion q;

	if (a == 0)
		throw domain_error("Division by 0");

	a = 1 / a;

	q._x = _x * a;
	q._y = _y * a;
	q._z = _z * a;
	q._w = _w * a;

	return q;
}

Quaternion &Quaternion::operator*=(float a)
{
	_x *= a;
	_y *= a;
	_z *= a;
	_w *= a;

	return *this;
}

Quaternion &Quaternion::operator/=(float a)
{
	if (a == 0)
		throw domain_error("Division by 0");

	a = 1 / a;

	_x *= a;
	_y *= a;
	_z *= a;
	_w *= a;

	return *this;
}

Quaternion Quaternion::operator+(const Quaternion &q) const
{
	Quaternion p = q;

	p._x += _x;
	p._y += _y;
	p._z += _z;
	p._w += _w;

	return p;
}

Quaternion Quaternion::operator-(const Quaternion &q) const
{
	Quaternion p = *this;

	p._x -= q._x;
	p._y -= q._y;
	p._z -= q._z;
	p._w -= q._w;

	return p;
}

Quaternion Quaternion::operator*(const Quaternion &q) const
{
	Quaternion p;

	p._x = _w * q._x + q._w * _x + _y * q._z - _z * q._y;
	p._y = _w * q._y + q._w * _y + _z * q._x - _x * q._z;
	p._z = _w * q._z + q._w * _z + _x * q._y - _y * q._x;

	p._w = _w * q._w - _x * q._x - _y * q._y - _z * q._z;

	return p;
}

Quaternion Quaternion::operator/(const Quaternion &q) const
{
	Quaternion p = q;

	p.invert();

	return *this * p;
}

Quaternion &Quaternion::operator+=(const Quaternion &q)
{
	_x += q._x;
	_y += q._y;
	_z += q._z;
	_w += q._w;

	return *this;
}

Quaternion &Quaternion::operator-=(const Quaternion &q)
{
	_x -= q._x;
	_y -= q._y;
	_z -= q._z;
	_w -= q._w;

	return *this;
}

Quaternion &Quaternion::operator*=(const Quaternion &q)
{
	return *this = *this * q;
}

Quaternion &Quaternion::operator/=(const Quaternion &q)
{
	return *this = *this / q;
}

void Quaternion::conjugate()
{
	_x = -_x;
	_y = -_y;
	_z = -_z;
}

void Quaternion::invert()
{
	float m = modulus2();

	if (m == 0)
		throw domain_error("Quaternion not invertible.");

	conjugate();
	*this *= 1 / m;
}

float Quaternion::modulus() const
{
	return sqrtf(_x * _x + _y * _y + _z * _z + _w * _w);
}

float Quaternion::modulus2() const
{
	return _x * _x + _y * _y + _z * _z + _w * _w;
}

Matrix4 Quaternion::toRotationMatrix() const
{
	Matrix4 m;

	m(0, 0) = 1 - 2 * (_y * _y + _z * _z);
	m(0, 1) = 2 * (_x * _y + _z * _w);
	m(0, 2) = 2 * (_x * _z - _y * _w);

	m(1, 0) = 2 * (_x * _y - _z * _w);
	m(1, 1) = 1 - 2 * (_x * _x + _z * _z);
	m(1, 2) = 2 * (_y * _z + _x * _w);

	m(2, 0) = 2 * (_x * _z + _y * _w);
	m(2, 1) = 2 * (_y * _z - _x * _w);
	m(2, 2) = 1 - 2 * (_x * _x + _y * _y);

	return m;
}

Quaternion Quaternion::slerp(const Quaternion &q1, const Quaternion &q2, float t)
{
	Quaternion q;
	float theta = acosf(q1._x * q2._x + q1._y * q2._y + q1._z * q2._z);

	q = q1 * sinf((1 - t) * theta) + q2 * sinf(t * theta);
	q /= sinf(theta);

	return q;
}
