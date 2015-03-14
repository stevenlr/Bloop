#ifndef GRAPHICS_OPENGL_SHADER_H
#define GRAPHICS_OPENGL_SHADER_H

#include <GL/glew.h>
#include <string>

class Shader {
public:
	enum Type : GLenum {
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER
	};

	Shader(const std::string &filename, Type type);
	~Shader();

	Shader(const Shader &shader) = delete;
	Shader &operator=(const Shader &shader) = delete;

	GLuint getId() const;

private:
	void loadSource(const std::string &filename);

	GLuint _id;
	Type _type;
};

#endif
