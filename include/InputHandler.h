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

	enum MouseButton : int {
		Attack,
		Interact,

		MOUSE_BUTTON_NB_ITEMS
	};

	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mousePositionCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void keyCallback(int key, int scancode, int action, int mods);
	void mousePositionCallback(float x, float y);
	void mouseButtonCallback(int button, int action, int mods);

	bool keyWasPressed(Key key);
	bool keyWasReleased(Key key);
	bool keyIsDown(Key key);
	void mouseMotion(float &dx, float &dy);
	void mousePosition(float &x, float &y);
	bool mouseButtonWasPressed(MouseButton button);
	bool mouseButtonWasReleased(MouseButton button);
	bool mouseButtonIsDown(MouseButton button);

	void poll();
	void update();

	static InputHandler &getInstance();

private:
	class ButtonState {
	public:
		ButtonState();

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

	ButtonState _keys[KEY_NB_ITEMS];
	std::map<int, Key> _invertConfigKey;
	int _configKey[KEY_NB_ITEMS];

	ButtonState _mouseButtons[MOUSE_BUTTON_NB_ITEMS];
	std::map<int, MouseButton> _invertConfigMouseButton;
	int _configMouseButton[MOUSE_BUTTON_NB_ITEMS];

	float _lastMouseX;
	float _lastMouseY;
	float _mouseX;
	float _mouseY;

	static InputHandler _instance;
	static bool _initialized;
};

#endif
