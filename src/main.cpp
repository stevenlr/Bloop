#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "graphics/opengl/Shader.h"

using namespace std;

int main(int argc, char *argv[])
{
	Logger::init(&cerr);

	GLFWwindow *window;

	if (!glfwInit()) {
		LOGERROR << "Couldn't initialize GLFW" << endl;
		return 1;
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "test", nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		LOGERROR << "Couldn't create GLFWwindow" << endl;
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		LOGERROR << "Couldn't initialize GLEW" << endl;
		return 1;
	}

	glfwSwapInterval(1);
	glViewport(0, 0, 1280, 720);
	glClearColor(0, 0, 0, 1);

	float vertices[] = {0, 0, 0, 1, 0, 0, 0, 1, 0};
	GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	Shader defaultShader("shaders/default.vert", "shaders/default.frag");

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
