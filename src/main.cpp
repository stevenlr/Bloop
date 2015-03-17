#include <iostream>
#include <cmath>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "InputHandler.h"

#include "graphics/opengl/ShaderProgram.h"
#include "graphics/opengl/Uniform.h"
#include "graphics/opengl/Buffer.h"
#include "graphics/opengl/VertexArray.h"
#include "graphics/opengl/ElementIndexArray.h"

#include "graphics/Camera.h"

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/MatrixFactory.h"
#include "maths/TransformPipeline.h"

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

	InputHandler &input = InputHandler::getInstance();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, InputHandler::keyCallback);
	glfwSetCursorPosCallback(window, InputHandler::mousePositionCallback);
	glfwSetMouseButtonCallback(window, InputHandler::mouseButtonCallback);

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
	defaultShader.bindAttribLocation("in_Color", 1);
	defaultShader.bindFragDataLocation("out_Color", 0);
	defaultShader.link();
	defaultShader.bind();

	defaultShader["u_f"].set1f(0.5f);

	VertexArray vao(VertexArray::Triangles, 36);

	Vector3 vertices[] = {
		{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
		{0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}
	};

	Vector3 colors[] = {
		{0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
		{0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}
	};

	GLubyte indices[] = {
		3, 1, 0, 0, 2, 3,
		4, 5, 7, 7, 6, 4,
		1, 3, 7, 7, 5, 1,
		6, 2, 0, 0, 4, 6,
		0, 1, 5, 5, 4, 0,
		7, 3, 2, 2, 6, 7
	};

	Buffer bufferPos(Buffer::Array, Buffer::StaticDraw);
	bufferPos.data(sizeof(vertices), vertices);

	Buffer bufferColor(Buffer::Array, Buffer::StaticDraw);
	bufferColor.data(sizeof(colors), colors);

	Buffer bufferIndex(Buffer::ElementArray, Buffer::StaticDraw);
	bufferIndex.data(sizeof(indices), indices);
	
	vao.addAttrib(0, VertexAttrib(&bufferPos, 3, VertexAttrib::Float));
	vao.addAttrib(1, VertexAttrib(&bufferColor, 3, VertexAttrib::Float));
	vao.setElementIndexArray(ElementIndexArray(&bufferIndex, ElementIndexArray::UnsignedByte));

	vao.bind();

	TransformPipeline tp;
	Camera camera({-3, 0.5, 0.5});

	tp.perspectiveProjection(70, 1280, 720, 0.1, 10000);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);

	bool running = true;

	while (running) {
		input.poll();

		if (glfwWindowShouldClose(window) || input.keyWasPressed(InputHandler::Quit))
			running = false;

		camera.update();
		tp.lookAt(camera);

		defaultShader["u_PvmMatrix"].setMatrix4(tp.getPVMMatrix());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		vao.drawElements();
		glfwSwapBuffers(window);
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

#ifndef NDEBUG
		cin.get();
#endif
	} catch (...) {
		LOGERROR << "Unknown exception thrown.";

#ifndef NDEBUG
		cin.get();
#endif
	}
}
