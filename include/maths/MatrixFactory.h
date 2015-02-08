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

	static Matrix4 orthographicProjection4x4(float left, float right, float bottom, float top, float near, float far);
	static Matrix4 perspectiveProjection4x4(float fov, float width, float height, float near, float far);
	static Matrix4 lookAt4x4(const Vector3 &position, const Vector3 &target, const Vector3 &up);
};

#endif
