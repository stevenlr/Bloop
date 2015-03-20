#include "graphics/Mesh.h"

#include <stdexcept>

using namespace std;

Mesh::Mesh(unsigned int nbFaces) :
		_nbFaces(nbFaces),
		_mainBuffer(Buffer::Array, Buffer::StaticDraw),
		_tangentsBuffer(Buffer::Array, Buffer::StaticDraw),
		_vao(VertexArray::Triangles, nbFaces * 3)
{
	if (nbFaces < 1)
		throw runtime_error("Creating mesh with less that 1 face.");

	int mainStride = (3 + 3 + 2) * sizeof(float);

	_vao.bind();
	_vao.addAttrib(0, VertexAttrib(&_mainBuffer, 3, VertexAttrib::Float, false, mainStride, 0, 0));
	_vao.addAttrib(1, VertexAttrib(&_mainBuffer, 3, VertexAttrib::Float, false, mainStride, reinterpret_cast<void *>(sizeof(float) * 3), 0));
	_vao.addAttrib(2, VertexAttrib(&_mainBuffer, 2, VertexAttrib::Float, false, mainStride, reinterpret_cast<void *>(sizeof(float) * (3 + 3)), 0));
	_vao.addAttrib(3, VertexAttrib(&_mainBuffer, 3, VertexAttrib::Float, false, 0, 0, 3));
	_vao.unbind();
}

void Mesh::setMainBufferData(float *data)
{
	int size = _nbFaces * 3 * (3 + 3 + 2) * sizeof(float);

	_mainBuffer.data(size, data);
}

void Mesh::setTangentsBufferData(float *data)
{
	int size = _nbFaces * 3 * sizeof(float);

	_tangentsBuffer.data(size, data);
}

void Mesh::draw() const
{
	_vao.bind();
	_vao.drawArrays();
	_vao.unbind();
}
