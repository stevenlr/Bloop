#include <iostream>
#include <cmath>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "graphics/opengl/ShaderProgram.h"
#include "graphics/opengl/Uniform.h"
#include "graphics/opengl/Buffer.h"
#include "graphics/opengl/VertexArray.h"
#include "graphics/opengl/ElementIndexArray.h"

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/MatrixFactory.h"

using namespace std;

void run(int argc, char *argv[])
{
	Logger::init(&cerr);

	GLFWwindow *window;

	if (!glfwInit())
		throw runtime_error("Couldn't initialize GLFW.");

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1280, 720, "test", nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		throw runtime_error("Couldn't create GLFWwindow.");
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		throw runtime_error("Couldn't initialize GLEW.");
	}

	glfwSwapInterval(1);
	glViewport(0, 0, 1280, 720);
	glClearColor(0, 0, 0, 1);

	ShaderProgram defaultShader("shaders/default.vert", "shaders/default.frag");
	defaultShader.bindAttribLocation("in_Position", 0);
	defaultShader.bindFragDataLocation("out_Color", 0);
	defaultShader.link();
	defaultShader.bind();

	defaultShader["u_f"].set1f(0.5f);

	VertexArray vao(VertexArray::Triangles, 6);

	Vector3 vertices[] = {{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0}};
	unsigned int indices[] = {0, 1, 2, 2, 1, 3};

	Buffer bufferPos(Buffer::Array, Buffer::StaticDraw);
	bufferPos.data(sizeof(vertices), vertices);

	Buffer bufferIndex(Buffer::ElementArray, Buffer::StaticDraw);
	bufferIndex.data(sizeof(indices), indices);
	
	vao.addAttrib(VertexAttrib(&bufferPos, 0, 3, VertexAttrib::Float));
	vao.setElementIndexArray(ElementIndexArray(&bufferIndex));

	vao.bind();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
	
		vao.drawElements();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	vao.unbind();

	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int argc, char *argv[])
{
	try {
		run(argc, argv);
	} catch (const exception &e) {
		LOGERROR << "Exception thrown : " << e.what() << endl;
	} catch (...) {
		LOGERROR << "Unknown exception thrown.";
	}

#ifndef NDEBUG
	cin.get();
#endif
}
