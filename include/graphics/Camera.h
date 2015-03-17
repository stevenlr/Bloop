#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include "maths/Matrix.h"
#include "maths/Vector.h"

class Camera {
public:
	Camera(const Vector3 &position = {0, 0, 0}, float hangle = 0, float vangle = 0);

	void update();
	Matrix4 toLookAtMatrix() const;

private:
	Vector3 _position;
	Vector3 _up;
	float _hangle;
	float _vangle;
};

#endif
