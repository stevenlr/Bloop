#ifndef MATHS_MATRIXFACTORY_H
#define MATHS_MATRIXFACTORY_H

#include "maths/Matrix.h"
#include "maths/Vector.h"
#include "maths/Quaternion.h"

class MatrixFactory {
public:
	static Matrix4 translation4x4(const Vector3 &offset);
	static Matrix4 translation4x4(float x, float y, float z);

	static Matrix4 scale4x4(const Vector3 &scale);
	static Matrix4 scale4x4(float a);
	static Matrix4 scale4x4(float x, float y, float z);

	static Matrix4 rotationX4x4(float a);
	static Matrix4 rotationY4x4(float a);
	static Matrix4 rotationZ4x4(float a);

	static Matrix4 rotation4x4(const Vector3 &axis, float angle);
	static Matrix4 rotation4x4(float ux, float uy, float uz, float angle);
	static Matrix4 rotation4x4(const Quaternion &q);
};

#endif
