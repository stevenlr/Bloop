#ifndef GRAPHICS_MESH_H
#define GRAPHICS_MESH_H

#include <GL/glew.h>

#include "graphics/opengl/Buffer.h"
#include "graphics/opengl/VertexArray.h"
#include "graphics/opengl/VertexAttrib.h"

class Mesh {
public:
	Mesh(unsigned int nbFaces);

	Mesh(const Mesh &mesh) = delete;
	Mesh &operator=(const Mesh &mesh) = delete;

	void setBufferData(float *data);
	void addAttrib(GLuint index, const VertexAttrib &attrib);

	void draw() const;
	void drawInstanced(int count) const;

private:
	unsigned int _nbFaces;
	Buffer _buffer;
	VertexArray _vao;
};

#endif
