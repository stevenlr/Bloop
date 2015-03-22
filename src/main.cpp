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
#include "graphics/opengl/Sampler.h"

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

	ShaderProgram cubemapShader("shaders/cubemap.vert", "shaders/cubemap.frag");
	cubemapShader.bindAttribLocation("in_Position", 0);
	cubemapShader.bindFragDataLocation("out_Color", 0);
	cubemapShader.link();

	TransformPipeline tp;
	Camera camera({-3, 0.5, 0.5});

	tp.perspectiveProjection(70, 1280, 720, 0.1f, 10000);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	bool running = true;

	Sampler samplerMipmap(Sampler::MinLinearMipmapLinear, Sampler::MagLinear, Sampler::Repeat);
	Sampler samplerCubemap(Sampler::MinLinearMipmapLinear, Sampler::MagLinear, Sampler::ClampToEdge);

	Mesh *suzanne = loadCobjModel("models/suzanne.cobj");
	Mesh *environmentCube = loadCobjModel("models/environment_cube.cobj");

	Texture *gravelDiffuse = loadPngTexture("textures/gravel-diffuse.png", true);
	Texture *gravelSpecular = loadPngTexture("textures/gravel-specular.png", true);
	Texture *gravelNormal = loadPngTexture("textures/gravel-normal.png", true);

	samplerMipmap.bind(1);
	samplerMipmap.bind(2);
	samplerMipmap.bind(3);

	gravelDiffuse->bind(1);
	gravelNormal->bind(2);
	gravelSpecular->bind(3);

	Cubemap *cubemap = loadPngCubemap({
		"textures/cubemap/posx.png",
		"textures/cubemap/negx.png",
		"textures/cubemap/negy.png",
		"textures/cubemap/posy.png",
		"textures/cubemap/posz.png",
		"textures/cubemap/negz.png"
	}, true);

	samplerCubemap.bind(4);
	cubemap->bind(4);

	defaultShader.bind();
	defaultShader["u_DiffuseTexture"].set1i(1);
	defaultShader["u_SpecularTexture"].set1i(3);
	defaultShader["u_NormalTexture"].set1i(2);
	defaultShader["u_Cubemap"].set1i(4);

	cubemapShader.bind();
	cubemapShader["u_Cubemap"].set1i(4);
	
	tp.rotationZ(1.7);

	while (running) {
		input.poll();

		if (glfwWindowShouldClose(window) || input.keyWasPressed(InputHandler::Quit))
			running = false;

		camera.update();
		tp.lookAt(camera);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		defaultShader.bind();
		defaultShader["u_PvmMatrix"].setMatrix4(tp.getPVMMatrix());
		defaultShader["u_ViewMatrix"].setMatrix4(tp.getViewMatrix());
		defaultShader["u_InverseViewMatrix"].setMatrix4(tp.getInverseViewMatrix());
		defaultShader["u_ModelViewMatrix"].setMatrix4(tp.getViewModelMatrix());
		defaultShader["u_NormalMatrix"].setMatrix3(tp.getNormalMatrix());
		suzanne->draw();

		tp.saveModel();
		tp.identity();
		cubemapShader.bind();
		cubemapShader["u_PvmMatrix"].setMatrix4(tp.getPVMMatrix());
		glDepthFunc(GL_LEQUAL);
		environmentCube->draw();
		glDepthFunc(GL_LESS);

		tp.restoreModel();

		glfwSwapBuffers(window);
	}

	delete environmentCube;
	delete suzanne;
	delete gravelDiffuse;
	delete gravelSpecular;
	delete gravelNormal;
	delete cubemap;

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
