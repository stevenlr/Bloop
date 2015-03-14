#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <map>

#include <GLFW/glfw3.h>

class InputHandler {
public:
	enum Key : int {
		Forward,
		Backward,
		Left,
		Right,
		Up,
		Down,
		Quit,

		KEY_NB_ITEMS
	};

	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mousePositionCallback(GLFWwindow* window, double dx, double dy);

	void keyCallback(int key, int scancode, int action, int mods);
	void mousePositionCallback(float dx, float dy);

	bool keyWasPressed(Key key);
	bool keyWasReleased(Key key);
	bool keyIsDown(Key key);
	void mouseMotion(float &dx, float &dy);
	void mousePosition(float &x, float &y);

	void poll();
	void update();

	static InputHandler *getInstance();

private:
	class KeyState {
	public:
		KeyState();

		void press();
		void release();
		void update();

		bool wasPressed();
		bool wasReleased();
		bool isDown();

	private:
		bool _currentState;
		bool _pressedCurrentTick;
		bool _releasedCurrentTick;
	};

	InputHandler();

	InputHandler(const InputHandler &ih) = delete;
	InputHandler &operator=(const InputHandler &ih) = delete;

	KeyState _keys[KEY_NB_ITEMS];
	std::map<int, Key> _invertConfig;
	int _config[KEY_NB_ITEMS];

	float _lastMouseX;
	float _lastMouseY;
	float _mouseX;
	float _mouseY;

	static InputHandler _instance;
	static bool _initialized;
};

#endif
