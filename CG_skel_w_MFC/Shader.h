#pragma once
#include <GL/gl.h>
#include <memory>

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;

class Shader
{
public:
	Shader(const std::string& filename, GLenum type);
	~Shader();
	GLuint get_shader_id();

private:
	GLuint _id;
	static std::string read_shader_source(const std::string& filename);

	Shader(const Shader& other) = delete;
	Shader(Shader&& other) noexcept = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept = delete;
};
