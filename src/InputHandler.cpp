#include "InputHandler.h"

using namespace std;

InputHandler InputHandler::_instance;
bool InputHandler::_initialized = false;

InputHandler *InputHandler::getInstance()
{
	if (!_initialized) {
		_initialized = true;
	}

	return &_instance;
}

InputHandler::InputHandler()
{
	_config[Forward] = GLFW_KEY_Z;
	_config[Backward] = GLFW_KEY_S;
	_config[Left] = GLFW_KEY_Q;
	_config[Right] = GLFW_KEY_D;
	_config[Up] = GLFW_KEY_SPACE;
	_config[Down] = GLFW_KEY_LEFT_SHIFT;
	_config[Quit] = GLFW_KEY_ESCAPE;

	for (int i = 0; i < KEY_NB_ITEMS; ++i) {
		_invertConfig.insert(make_pair(_config[i], static_cast<Key>(i)));
	}
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	_instance.keyCallback(key, scancode, action, mods);
}

void InputHandler::keyCallback(int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS && action != GLFW_RELEASE)
		return;

	map<int, Key>::iterator it = _invertConfig.find(key);

	if (it != _invertConfig.end()) {
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

void InputHandler::poll()
{
	glfwPollEvents();
}

void InputHandler::update()
{
	for (int i = 0; i < KEY_NB_ITEMS; ++i) {
		_keys[i].update();
	}
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

InputHandler::KeyState::KeyState()
{
	_currentState = false;
	_pressedCurrentTick = false;
	_releasedCurrentTick = false;
}

void InputHandler::KeyState::press()
{
	_pressedCurrentTick = true;
	_currentState = true;
}

void InputHandler::KeyState::release()
{
	_currentState = false;
	_releasedCurrentTick = true;
}

void InputHandler::KeyState::update()
{
	_pressedCurrentTick = false;
	_releasedCurrentTick = false;
}

bool InputHandler::KeyState::wasPressed()
{
	return _pressedCurrentTick;
}

bool InputHandler::KeyState::wasReleased()
{
	return _releasedCurrentTick;
}

bool InputHandler::KeyState::isDown()
{
	return _currentState;
}
