#include "InputHandler.h"

using namespace std;

InputHandler InputHandler::_instance;
bool InputHandler::_initialized = false;

InputHandler &InputHandler::getInstance()
{
	if (!_initialized) {
		_initialized = true;
	}

	return _instance;
}

InputHandler::InputHandler()
{
	_configKey[Forward] = GLFW_KEY_Z;
	_configKey[Backward] = GLFW_KEY_S;
	_configKey[Left] = GLFW_KEY_Q;
	_configKey[Right] = GLFW_KEY_D;
	_configKey[Up] = GLFW_KEY_SPACE;
	_configKey[Down] = GLFW_KEY_LEFT_SHIFT;
	_configKey[Quit] = GLFW_KEY_ESCAPE;

	_configMouseButton[Attack] = GLFW_MOUSE_BUTTON_LEFT;
	_configMouseButton[Interact] = GLFW_MOUSE_BUTTON_RIGHT;

	for (int i = 0; i < KEY_NB_ITEMS; ++i) {
		_invertConfigKey.insert(make_pair(_configKey[i], static_cast<Key>(i)));
	}

	for (int i = 0; i < MOUSE_BUTTON_NB_ITEMS; ++i) {
		_invertConfigMouseButton.insert(make_pair(_configMouseButton[i], static_cast<MouseButton>(i)));
	}

	_lastMouseX = 0;
	_lastMouseY = 0;
	_mouseX = 0;
	_mouseY = 0;
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	_instance.keyCallback(key, scancode, action, mods);
}

void InputHandler::mousePositionCallback(GLFWwindow* window, double x, double y)
{
	_instance.mousePositionCallback(x, y);
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	_instance.mouseButtonCallback(button, action, mods);
}

void InputHandler::keyCallback(int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS && action != GLFW_RELEASE)
		return;

	map<int, Key>::iterator it = _invertConfigKey.find(key);

	if (it != _invertConfigKey.end()) {
		Key key = it->second;

		switch (action) {
			case GLFW_PRESS:
				_keys[key].press();
				break;
			case GLFW_RELEASE:
				_keys[key].release();
				break;
		}
	}
}

void InputHandler::mousePositionCallback(float x, float y)
{
	_mouseX = x;
	_mouseY = y;
}

void InputHandler::mouseButtonCallback(int button, int action, int mods)
{
	if (action != GLFW_PRESS && action != GLFW_RELEASE)
		return;

	map<int, MouseButton>::iterator it = _invertConfigMouseButton.find(button);

	if (it != _invertConfigMouseButton.end()) {
		MouseButton button = it->second;

		switch (action) {
			case GLFW_PRESS:
				_mouseButtons[button].press();
				break;
			case GLFW_RELEASE:
				_mouseButtons[button].release();
				break;
		}
	}
}

void InputHandler::poll()
{
	update();
	glfwPollEvents();
}

void InputHandler::update()
{
	for (int i = 0; i < KEY_NB_ITEMS; ++i) {
		_keys[i].update();
	}

	_lastMouseX = _mouseX;
	_lastMouseY = _mouseY;
}

bool InputHandler::keyWasPressed(Key key)
{
	if (key < 0 || key >= KEY_NB_ITEMS)
		throw runtime_error("Key symbol out of range.");

	return _keys[key].wasPressed();
}

bool InputHandler::keyWasReleased(Key key)
{
	if (key < 0 || key >= KEY_NB_ITEMS)
		throw runtime_error("Key symbol out of range.");

	return _keys[key].wasReleased();
}

bool InputHandler::keyIsDown(Key key)
{
	if (key < 0 || key >= KEY_NB_ITEMS)
		throw runtime_error("Key symbol out of range.");

	return _keys[key].isDown();
}

void InputHandler::mouseMotion(float &dx, float &dy)
{
	dx = _mouseX - _lastMouseX;
	dy = _mouseY - _lastMouseY;
}

void InputHandler::mousePosition(float &x, float &y)
{
	x = _mouseX;
	y = _mouseY;
}

bool InputHandler::mouseButtonWasPressed(MouseButton button)
{
	if (button < 0 || button >= MOUSE_BUTTON_NB_ITEMS)
		throw runtime_error("Mouse button symbol out of range.");

	return _mouseButtons[button].wasPressed();
}

bool InputHandler::mouseButtonWasReleased(MouseButton button)
{
	if (button < 0 || button >= MOUSE_BUTTON_NB_ITEMS)
		throw runtime_error("Mouse button symbol out of range.");

	return _mouseButtons[button].wasReleased();
}

bool InputHandler::mouseButtonIsDown(MouseButton button)
{
	if (button < 0 || button >= MOUSE_BUTTON_NB_ITEMS)
		throw runtime_error("Mouse button symbol out of range.");

	return _mouseButtons[button].isDown();
}

InputHandler::ButtonState::ButtonState()
{
	_currentState = false;
	_pressedCurrentTick = false;
	_releasedCurrentTick = false;
}

void InputHandler::ButtonState::press()
{
	_pressedCurrentTick = true;
	_currentState = true;
}

void InputHandler::ButtonState::release()
{
	_currentState = false;
	_releasedCurrentTick = true;
}

void InputHandler::ButtonState::update()
{
	_pressedCurrentTick = false;
	_releasedCurrentTick = false;
}

bool InputHandler::ButtonState::wasPressed()
{
	return _pressedCurrentTick;
}

bool InputHandler::ButtonState::wasReleased()
{
	return _releasedCurrentTick;
}

bool InputHandler::ButtonState::isDown()
{
	return _currentState;
}
