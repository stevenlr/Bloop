#ifndef MATHS_QUATERNION_H
#define MATHS_QUATERNION_H

#include "maths/Vector.h"
#include "maths/Matrix.h"

class Quaternion {
public:
	Quaternion();
	Quaternion(const Quaternion &q);
	Quaternion(const Vector3 &v, bool point = true);
	Quaternion(const Vector4 &v);
	Quaternion(const Vector3 &axis, float angle);
	Quaternion(float ux, float uy, float uz, float angle);

	Quaternion &operator=(const Quaternion &q);

	Quaternion operator*(float a) const;
	Quaternion operator/(float a) const;
	Quaternion &operator*=(float a);
	Quaternion &operator/=(float a);

	Quaternion operator+(const Quaternion &q) const;
	Quaternion operator-(const Quaternion &q) const;
	Quaternion operator*(const Quaternion &q) const;
	Quaternion operator/(const Quaternion &q) const;
	Quaternion &operator+=(const Quaternion &q);
	Quaternion &operator-=(const Quaternion &q);
	Quaternion &operator*=(const Quaternion &q);
	Quaternion &operator/=(const Quaternion &q);

	void conjugate();
	void invert();
	float modulus() const;
	float modulus2() const;
	Matrix4 toRotationMatrix() const;

	static Quaternion slerp(const Quaternion &q1, const Quaternion &q2, float t);

private:
	float _x;
	float _y;
	float _z;
	float _w;
};

#endif
