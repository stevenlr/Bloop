#include <iostream>
#include <cmath>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

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
#include "graphics/opengl/Texture.h"
#include "graphics/opengl/Framebuffer.h"

#include "graphics/Camera.h"
#include "graphics/CobjLoader.h"
#include "graphics/PngLoader.h"
#include "graphics/Mesh.h"

#include "maths/Vector.h"
#include "maths/Matrix.h"
#include "maths/MatrixFactory.h"
#include "maths/TransformPipeline.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

using namespace std;

void environmentMapping(GLFWwindow *window, InputHandler &input)
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

	tp.perspectiveProjection(70, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 10000);

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
}

void deferredShading(GLFWwindow *window, InputHandler &input)
{
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0, 0, 0, 1);

	TransformPipeline tpScene;
	Camera camera({-3, 0.5, 0.5});

	tpScene.perspectiveProjection(70, WINDOW_WIDTH, WINDOW_HEIGHT, 0.1f, 10000);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);

	bool running = true;

	// ----- Quad ---------

	float quadCoords[] = {0, 0, 1, 0, 1, 1, 0, 1};
	unsigned int quadIndices[] = {0, 1, 3, 3, 1, 2};

	VertexArray quadVao(VertexArray::Triangles, 6);

	Buffer quadCoordsBuffer(Buffer::Array, Buffer::StaticDraw);
	quadCoordsBuffer.data(8 * sizeof(float), reinterpret_cast<const void*>(quadCoords));

	Buffer quadIndicesBuffer(Buffer::ElementArray, Buffer::StaticDraw);
	quadIndicesBuffer.data(6 * sizeof(unsigned int), reinterpret_cast<const void*>(quadIndices));

	quadVao.addAttrib(0, VertexAttrib(&quadCoordsBuffer, 2, VertexAttrib::Float));
	quadVao.setElementIndexArray(ElementIndexArray(&quadIndicesBuffer));

	// ----- Cubemap -------

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	Sampler samplerCubemap(Sampler::MinLinearMipmapLinear, Sampler::MagLinear, Sampler::ClampToEdge);
	Mesh *environmentCube = loadCobjModel("models/environment_cube.cobj");

	ShaderProgram cubemapShader("shaders/cubemap.vert", "shaders/cubemap.frag");
	cubemapShader.bindAttribLocation("in_Position", 0);
	cubemapShader.bindFragDataLocation("out_Color", 0);
	cubemapShader.link();

	Cubemap *cubemap = loadPngCubemap({
		"textures/cubemap/posx.png",
		"textures/cubemap/negx.png",
		"textures/cubemap/negy.png",
		"textures/cubemap/posy.png",
		"textures/cubemap/posz.png",
		"textures/cubemap/negz.png"
	}, true);

	cubemapShader.bind();
	cubemapShader["u_Cubemap"].set1i(1);
	cubemapShader.unbind();

	// ----- Scene ---------

	Sampler samplerMipmap(Sampler::MinLinearMipmapLinear, Sampler::MagLinear, Sampler::Repeat);
	Sampler samplerLinear(Sampler::MinLinear, Sampler::MagLinear, Sampler::ClampToEdge);

	Mesh *suzanne = loadCobjModel("models/suzanne.cobj");
	Mesh *sphere = loadCobjModel("models/light_sphere.cobj");

	Texture *gravelDiffuse = loadPngTexture("textures/gravel-diffuse.png", true);
	Texture *gravelNormal = loadPngTexture("textures/gravel-normal.png", true);
	
	tpScene.rotationZ(1.7);

	Texture *gbufferDiffuse = new Texture(WINDOW_WIDTH, WINDOW_HEIGHT, Texture::RGB32f, Texture::RGB, Texture::Float);
	Texture *gbufferNormal = new Texture(WINDOW_WIDTH, WINDOW_HEIGHT, Texture::RGB32f, Texture::RGB, Texture::Float);
	Texture *gbufferPosition = new Texture(WINDOW_WIDTH, WINDOW_HEIGHT, Texture::RGB32f, Texture::RGB, Texture::Float);
	Renderbuffer *gbufferDepth = new Renderbuffer(WINDOW_WIDTH, WINDOW_HEIGHT, Renderbuffer::Depth24Stencil8);

	Framebuffer *framebuffer = new Framebuffer();
	framebuffer->bind(Framebuffer::DrawFramebuffer);
	framebuffer->attachTexture(*gbufferDiffuse, Framebuffer::Color0);
	framebuffer->attachTexture(*gbufferNormal, Framebuffer::Color1);
	framebuffer->attachTexture(*gbufferPosition, Framebuffer::Color2);
	framebuffer->attachRenderbuffer(*gbufferDepth, Framebuffer::DepthStencil);
	framebuffer->drawBuffers({Framebuffer::Color0, Framebuffer::Color1, Framebuffer::Color2});
	framebuffer->unbind(Framebuffer::DrawFramebuffer);

	ShaderProgram geometryPassShader("shaders/geometry_pass.vert", "shaders/geometry_pass.frag");
	geometryPassShader.bindAttribLocation("in_Position", 0);
	geometryPassShader.bindAttribLocation("in_Normal", 1);
	geometryPassShader.bindAttribLocation("in_TextureCoords", 2);
	geometryPassShader.bindAttribLocation("in_Tangent", 3);
	geometryPassShader.bindFragDataLocation("out_Diffuse", 0);
	geometryPassShader.bindFragDataLocation("out_Normal", 1);
	geometryPassShader.bindFragDataLocation("out_Position", 2);
	geometryPassShader.link();

	geometryPassShader.bind();
	geometryPassShader["u_DiffuseTexture"].set1i(1);
	geometryPassShader["u_NormalTexture"].set1i(2);
	geometryPassShader.unbind();

	ShaderProgram instancedGeometryPassShader("shaders/instanced_geometry_pass.vert", "shaders/geometry_pass.frag");
	instancedGeometryPassShader.bindAttribLocation("in_Position", 0);
	instancedGeometryPassShader.bindAttribLocation("in_Normal", 1);
	instancedGeometryPassShader.bindAttribLocation("in_TextureCoords", 2);
	instancedGeometryPassShader.bindAttribLocation("in_Tangent", 3);
	instancedGeometryPassShader.bindAttribLocation("in_InstancePosition", 4);
	instancedGeometryPassShader.bindFragDataLocation("out_Diffuse", 0);
	instancedGeometryPassShader.bindFragDataLocation("out_Normal", 1);
	instancedGeometryPassShader.bindFragDataLocation("out_Position", 2);
	instancedGeometryPassShader.link();

	instancedGeometryPassShader.bind();
	instancedGeometryPassShader["u_DiffuseTexture"].set1i(1);
	instancedGeometryPassShader["u_NormalTexture"].set1i(2);
	instancedGeometryPassShader.unbind();

	ShaderProgram directionalLightingPassShader("shaders/directional_lighting_pass.vert", "shaders/directional_lighting_pass.frag");
	directionalLightingPassShader.bindAttribLocation("in_Position", 0);
	directionalLightingPassShader.bindFragDataLocation("out_Color", 0);
	directionalLightingPassShader.link();

	directionalLightingPassShader.bind();
	directionalLightingPassShader["u_DiffuseTexture"].set1i(1);
	directionalLightingPassShader["u_NormalTexture"].set1i(2);
	directionalLightingPassShader["u_PositionTexture"].set1i(3);
	directionalLightingPassShader.unbind();

	ShaderProgram pointLightingPassShader("shaders/point_lighting_pass.vert", "shaders/point_lighting_pass.frag");
	pointLightingPassShader.bindAttribLocation("in_Position", 0);
	pointLightingPassShader.bindFragDataLocation("out_Color", 0);
	pointLightingPassShader.link();

	pointLightingPassShader.bind();
	pointLightingPassShader["u_DiffuseTexture"].set1i(1);
	pointLightingPassShader["u_NormalTexture"].set1i(2);
	pointLightingPassShader["u_PositionTexture"].set1i(3);
	pointLightingPassShader.unbind();

	ShaderProgram pointLightingBoundingSphereShader("shaders/point_light_bounding_sphere.vert", "shaders/point_light_bounding_sphere.frag");
	pointLightingBoundingSphereShader.bindAttribLocation("in_Position", 0);
	pointLightingBoundingSphereShader.bindFragDataLocation("out_Color", 0);
	pointLightingBoundingSphereShader.link();

	Buffer instPosBuffer(Buffer::Array, Buffer::StaticDraw);
	const int nbInstances = 1000;

	{
		Vector3 pos[nbInstances];

		for (int i = 0; i < nbInstances; ++i) {
			float x = (static_cast<float>(rand()) / RAND_MAX - 0.5) * 40;
			float y = (static_cast<float>(rand()) / RAND_MAX - 0.5) * 40;
			float z = (static_cast<float>(rand()) / RAND_MAX - 0.5) * 40;

			pos[i] = {x, y, z};
		}

		instPosBuffer.data(nbInstances * sizeof(Vector3), reinterpret_cast<const void*>(pos));
	}

	sphere->addAttrib(4, VertexAttrib(&instPosBuffer, 3, VertexAttrib::Float, false, 0, 0, 1));

	const int nbPointLights = 30;
	Vector4 pointLightPositions[nbPointLights];
	Vector3 pointLightColor[nbPointLights];
	float pointLightIntensity[nbPointLights];
	float pointLightScale[nbPointLights];

	for (int i = 0; i < nbPointLights; ++i) {
		float x = (static_cast<float>(rand()) / RAND_MAX - 0.5) * 20;
		float y = (static_cast<float>(rand()) / RAND_MAX - 0.5) * 20;
		float z = (static_cast<float>(rand()) / RAND_MAX - 0.5) * 20;

		float r = (static_cast<float>(rand()) / RAND_MAX + 0.5) / 1.5;
		float g = (static_cast<float>(rand()) / RAND_MAX + 0.5) / 1.5;
		float b = (static_cast<float>(rand()) / RAND_MAX + 0.5) / 1.5;

		float intensity = (static_cast<float>(rand()) / RAND_MAX + 0.5) / 1.5 * 10;
		float vmax = max(max(r, g), b);
		float scale = pow(intensity * vmax * 256, 0.25) + 1;

		pointLightPositions[i] = {x, y, z, 1};
		pointLightColor[i] = {r, g, b};
		pointLightIntensity[i] = intensity;
		pointLightScale[i] = scale;
	}

	while (running) {
		input.poll();

		if (glfwWindowShouldClose(window) || input.keyWasPressed(InputHandler::Quit))
			running = false;

		camera.update();
		tpScene.lookAt(camera);
		tpScene.identity();

		// ----- Geometry pass -----

		geometryPassShader.bind();
		framebuffer->bind(Framebuffer::DrawFramebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xff);
		geometryPassShader["u_PvmMatrix"].setMatrix4(tpScene.getPVMMatrix());
		geometryPassShader["u_ViewMatrix"].setMatrix4(tpScene.getViewMatrix());
		geometryPassShader["u_InverseViewMatrix"].setMatrix4(tpScene.getInverseViewMatrix());
		geometryPassShader["u_ModelViewMatrix"].setMatrix4(tpScene.getViewModelMatrix());
		geometryPassShader["u_NormalMatrix"].setMatrix3(tpScene.getNormalMatrix());
		samplerMipmap.bind(1);
		samplerMipmap.bind(2);
		gravelDiffuse->bind(1);
		gravelNormal->bind(2);
		suzanne->draw();

		instancedGeometryPassShader.bind();
		instancedGeometryPassShader["u_PvmMatrix"].setMatrix4(tpScene.getPVMMatrix());
		instancedGeometryPassShader["u_ViewMatrix"].setMatrix4(tpScene.getViewMatrix());
		instancedGeometryPassShader["u_InverseViewMatrix"].setMatrix4(tpScene.getInverseViewMatrix());
		instancedGeometryPassShader["u_ModelViewMatrix"].setMatrix4(tpScene.getViewModelMatrix());
		instancedGeometryPassShader["u_NormalMatrix"].setMatrix3(tpScene.getNormalMatrix());
		sphere->drawInstanced(nbInstances);

		framebuffer->unbind(Framebuffer::DrawFramebuffer);

		// ----- Point lights pass -----

		samplerLinear.bind(1);
		samplerLinear.bind(2);
		samplerLinear.bind(3);
		gbufferDiffuse->bind(1);
		gbufferNormal->bind(2);
		gbufferPosition->bind(3);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_ADD);

		framebuffer->bind(Framebuffer::ReadFramebuffer);
		glBlitFramebuffer(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		framebuffer->unbind(Framebuffer::ReadFramebuffer);

		for (int i = 0; i < nbPointLights; ++i) {
			glClear(GL_STENCIL_BUFFER_BIT);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glStencilFunc(GL_ALWAYS, 1, 0xff);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR, GL_KEEP);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR, GL_KEEP);
			pointLightingBoundingSphereShader.bind();
			pointLightingBoundingSphereShader["u_PvmMatrix"].setMatrix4(tpScene.getPVMMatrix());
			pointLightingBoundingSphereShader["u_Position"].set4f(pointLightPositions[i]);
			pointLightingBoundingSphereShader["u_Scale"].set1f(pointLightScale[i]);
			sphere->draw();

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_NOTEQUAL, 0, 0xff);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			pointLightingPassShader.bind();
			pointLightingPassShader["u_LightPosition"].set4f(tpScene.getViewMatrix() * pointLightPositions[i]);
			pointLightingPassShader["u_LightColor"].set3f(pointLightColor[i]);
			pointLightingPassShader["u_LightIntensity"].set1f(pointLightScale[i]);
			quadVao.bind();
			quadVao.drawElements();
		}

		// ----- Copy stencil ------

		framebuffer->bind(Framebuffer::ReadFramebuffer);
		glBlitFramebuffer(0, 0, 1280, WINDOW_HEIGHT, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		framebuffer->unbind(Framebuffer::ReadFramebuffer);

		// ----- Directional lights pass -----

		Vector3 directionalLightDirs[] = {{0, 1, 1}, {0, -1, -1}};
		Vector3 directionalLightColors[] = {{0, 0.1f, 0.1f}, {0.1f, 0.1f, 0}};
		
		glStencilFunc(GL_ALWAYS, 1, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		
		directionalLightingPassShader.bind();
		directionalLightingPassShader["u_ViewMatrix"].setMatrix4(tpScene.getViewMatrix());
		quadVao.bind();

		for (int i = 0; i < 2; ++i) {
			directionalLightingPassShader["u_LightDir"].set3f(directionalLightDirs[i]);
			directionalLightingPassShader["u_LightColor"].set3f(directionalLightColors[i]);
			quadVao.drawElements();
		}

		quadVao.unbind();
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_ONE, GL_ZERO);

		// ----- Cubemap pass -----

		tpScene.saveModel();
		tpScene.identity();
		cubemapShader.bind();
		cubemapShader["u_PvmMatrix"].setMatrix4(tpScene.getPVMMatrix());
		tpScene.restoreModel();

		cubemapShader.bind();
		samplerCubemap.bind(1);
		cubemap->bind(1);

		glStencilFunc(GL_NOTEQUAL, 1, 0xff);

		glDepthFunc(GL_LEQUAL);
		environmentCube->draw();
		glDepthFunc(GL_LESS);

		glStencilFunc(GL_ALWAYS, 0, 0xff);
		glEnable(GL_CULL_FACE);

		glfwSwapBuffers(window);
	}

	delete environmentCube;
	delete suzanne;
	delete sphere;
	delete gravelDiffuse;
	delete gravelNormal;
	
	delete framebuffer;

	delete cubemap;
	delete gbufferDiffuse;
	delete gbufferPosition;
	delete gbufferNormal;
	delete gbufferDepth;
}

void run(int argc, char *argv[])
{
	Logger::init(&cerr);

	GLFWwindow *window;
	srand(time(nullptr));

	if (!glfwInit())
		throw runtime_error("Couldn't initialize GLFW.");

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "test", nullptr, nullptr);

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
	
	deferredShading(window, input);

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
