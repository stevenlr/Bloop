#ifndef MATHS_MATRIXFACTORY_H
#define MATHS_MATRIXFACTORY_H

#include "maths/Matrix.h"
#include "maths/Vector.h"

class MatrixFactory {
public:
	static Matrix4 translation4x4(const Vector3 &offset);
	static Matrix4 translation4x4(float x, float y, float z);

	static Matrix4 scale4x4(const Vector3 &scale);
	static Matrix4 scale4x4(float a);
	static Matrix4 scale4x4(float x, float y, float z);
};

#endif
