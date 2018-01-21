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
	GLuint get_id() const;
	std::string get_name() const;
	static const std::string SHADER_PATH;

private:
	GLuint _id;
	std::string _name;
	static std::string read_shader_source(const std::string& filename);

	Shader(const Shader& other) = delete;
	Shader(Shader&& other) noexcept = delete;
	Shader& operator=(const Shader& other) = delete;
	Shader& operator=(Shader&& other) noexcept = delete;
};
