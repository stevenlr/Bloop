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
#include "graphics/CobjLoader.h"
#include "graphics/PngLoader.h"
#include "graphics/Mesh.h"

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
	defaultShader.bindAttribLocation("in_Normal", 1);
	defaultShader.bindAttribLocation("in_TextureCoords", 2);
	defaultShader.bindAttribLocation("in_Tangent", 3);
	defaultShader.bindFragDataLocation("out_Color", 0);
	defaultShader.link();
	defaultShader.bind();

	TransformPipeline tp;
	Camera camera({-3, 0.5, 0.5});

	tp.perspectiveProjection(70, 1280, 720, 0.1f, 10000);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);

	bool running = true;

	Mesh *suzanne = loadCobjModel("models/suzanne.cobj");
	Texture *gravelDiffuse = loadPngTexture("textures/gravel-diffuse.png");
	Texture *gravelSpecular = loadPngTexture("textures/gravel-specular.png");
	Texture *gravelNormal = loadPngTexture("textures/gravel-normal.png");

	gravelDiffuse->bind(1);
	gravelNormal->bind(2);
	gravelSpecular->bind(3);

	defaultShader["u_DiffuseTexture"].set1i(1);
	defaultShader["u_SpecularTexture"].set1i(3);
	defaultShader["u_NormalTexture"].set1i(2);

	while (running) {
		input.poll();

		if (glfwWindowShouldClose(window) || input.keyWasPressed(InputHandler::Quit))
			running = false;

		camera.update();
		tp.lookAt(camera);

		defaultShader["u_PvmMatrix"].setMatrix4(tp.getPVMMatrix());
		defaultShader["u_ViewMatrix"].setMatrix4(tp.getViewMatrix());
		defaultShader["u_ModelViewMatrix"].setMatrix4(tp.getViewModelMatrix());
		defaultShader["u_NormalMatrix"].setMatrix3(tp.getNormalMatrix());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		suzanne->draw();

		glfwSwapBuffers(window);
	}

	delete suzanne;
	delete gravelDiffuse;
	delete gravelSpecular;
	delete gravelNormal;

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
