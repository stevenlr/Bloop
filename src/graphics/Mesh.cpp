#include "graphics/Mesh.h"

#include <stdexcept>

using namespace std;

Mesh::Mesh(unsigned int nbFaces) :
		_nbFaces(nbFaces),
		_buffer(Buffer::Array, Buffer::StaticDraw),
		_vao(VertexArray::Triangles, nbFaces * 3)
{
	if (nbFaces < 1)
		throw runtime_error("Creating mesh with less that 1 face.");

	int mainStride = (3 + 3 + 2 + 3) * sizeof(float);

	_vao.bind();
	_vao.addAttrib(0, VertexAttrib(&_buffer, 3, VertexAttrib::Float, false, mainStride, 0));
	_vao.addAttrib(1, VertexAttrib(&_buffer, 3, VertexAttrib::Float, false, mainStride, reinterpret_cast<void *>(sizeof(float) * 3)));
	_vao.addAttrib(2, VertexAttrib(&_buffer, 2, VertexAttrib::Float, false, mainStride, reinterpret_cast<void *>(sizeof(float) * (3 + 3))));
	_vao.addAttrib(3, VertexAttrib(&_buffer, 3, VertexAttrib::Float, false, mainStride, reinterpret_cast<void *>(sizeof(float) * (3 + 3 + 2))));
	_vao.unbind();
}

void Mesh::setBufferData(float *data)
{
	int size = _nbFaces * 3 * (3 + 3 + 2 + 3) * sizeof(float);

	_buffer.data(size, data);
}

void Mesh::addAttrib(GLuint index, const VertexAttrib &attrib)
{
	if (index < 4)
		throw runtime_error("Overriding mesh data with new vertex attrib.");

	_vao.addAttrib(index, attrib);
}

void Mesh::draw() const
{
	_vao.bind();
	_vao.drawArrays();
	_vao.unbind();
}

void Mesh::drawInstanced(int count) const
{
	_vao.bind();
	_vao.drawArraysInstanced(count);
	_vao.unbind();
}
