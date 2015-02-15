#ifndef GRAPHICS_OPENGL_VERTEXARRAY_H
#define GRAPHICS_OPENGL_VERTEXARRAY_H

#include "GL/glew.h"

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

	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray &vao) = delete;
	VertexArray &operator=(const VertexArray &vao) = delete;

	void bind() const;
	void unbind() const;

	void setMode(DrawMode mode);
	void setOffset(GLint offset);
	void setCount(GLint count);

	void drawArrays() const;

private:
	GLuint _id;
	GLenum _mode;
	GLint _offset;
	GLsizei _count;

	static GLuint _boundVertexArray;
	static bool _enabledAttribs[16];
};

#endif
