#ifndef GRAPHICS_OPENGL_VERTEXARRAY_H
#define GRAPHICS_OPENGL_VERTEXARRAY_H

#include <GL/glew.h>
#include <vector>
#include <cstdint>

#include "graphics/opengl/VertexAttrib.h"
#include "graphics/opengl/ElementIndexArray.h"

class VertexArray {
public:
	enum DrawMode : GLenum {
		Points = GL_POINTS,
		Lines = GL_LINES,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP,
		LinesAdjacency = GL_LINES_ADJACENCY,
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		Triangles = GL_TRIANGLES,
		TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
		TrianglesAdjacency = GL_TRIANGLES_ADJACENCY
	};

	VertexArray(DrawMode mode, GLint count, GLint offset = 0);
	~VertexArray();

	VertexArray(const VertexArray &vao) = delete;
	VertexArray &operator=(const VertexArray &vao) = delete;

	void bind() const;
	void unbind() const;

	void setMode(DrawMode mode);
	void setOffset(GLint offset);
	void setCount(GLint count);

	void addAttrib(GLuint index, const VertexAttrib &attrib);
	void setElementIndexArray(const ElementIndexArray &eia);

	void drawArrays() const;
	void drawElements() const;
	void drawArraysInstanced(int count) const;
	void drawElementsInstanced(int count) const;

private:
	GLuint _id;
	GLenum _mode;
	GLint _offset;
	GLsizei _count;
	bool _useEia;
	ElementIndexArray::Type _eiaType;

	static GLuint _boundVertexArray;
};

#endif
