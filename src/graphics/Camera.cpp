#include "graphics/Camera.h"

#include <algorithm>
#include <cmath>

#include "maths/MatrixFactory.h"
#include "maths/MathsConsts.h"
#include "InputHandler.h"

using namespace std;

Camera::Camera(const Vector3 &position, float hangle, float vangle) :
		_position(position), _up({0, 0, 1}), _hangle(hangle), _vangle(vangle)
{
	_up.normalize();
}

void Camera::update()
{
	InputHandler &input = InputHandler::getInstance();
	const float sensitivity = 0.001f;
	const float speed = 0.1f;
	const float vangleLimit = (PI / 2) - 0.01f;

	float dx, dy;
	input.mouseMotion(dx, dy);

	_hangle -= sensitivity * dx;
	_vangle -= sensitivity * dy;
	_vangle = min(max(_vangle, -vangleLimit), vangleLimit);

	Vector3 forward, right;
	forward[0] = cos(_hangle);
	forward[1] = sin(_hangle);
	right[0] = forward[1];
	right[1] = -forward[0];

	Vector3 direction;

	if (input.keyIsDown(InputHandler::Left))
		direction -= right;

	if (input.keyIsDown(InputHandler::Right))
		direction += right;

	if (input.keyIsDown(InputHandler::Forward))
		direction += forward;

	if (input.keyIsDown(InputHandler::Backward))
		direction -= forward;

	if (input.keyIsDown(InputHandler::Up))
		direction += _up;

	if (input.keyIsDown(InputHandler::Down))
		direction -= _up;

	try {
		direction.normalize();
	} catch (exception e) {
		return;
	}

	_position += direction * speed;
}

Matrix4 Camera::toLookAtMatrix() const
{
	float ch = cos(_hangle);
	float sh = sin(_hangle);
	float cv = cos(_vangle);
	float sv = sin(_vangle);

	Vector3 forward = {cv * ch, cv * sh, sv};
	Vector3 target = _position + forward;

	return MatrixFactory::lookAt4x4(_position, target, _up);
}
