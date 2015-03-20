#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <GL/glew.h>

#include "graphics/opengl/Buffer.h"
#include "graphics/opengl/VertexArray.h"

class Mesh {
public:
	Mesh(unsigned int nbFaces);

	Mesh(const Mesh &mesh) = delete;
	Mesh &operator=(const Mesh &mesh) = delete;

	void setMainBufferData(float *data);
	void setTangentsBufferData(float *data);
	void draw() const;

private:
	unsigned int _nbFaces;
	Buffer _mainBuffer;
	Buffer _tangentsBuffer;
	VertexArray _vao;
};

#endif
