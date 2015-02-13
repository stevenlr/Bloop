#ifndef GRAPHICS_OPENGL_SHADER_H_
#define GRAPHICS_OPENGL_SHADER_H_

#include <GL/glew.h>
#include <string>

class Shader {
public:
	enum Type : GLenum {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER
	};

	Shader(const char *filename, Type type);
	~Shader();

	Shader(const Shader &shader) = delete;
	Shader &operator=(const Shader &shader) = delete;

	GLuint getId();

private:
	void loadSource(const char *filename);

	GLuint _id;
	Type _type;
};

#endif
