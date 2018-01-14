#pragma once
#include <string>
#include "Shader.h"
#include "mat.h"

class ShaderProgram;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

class ShaderProgram
{
public:
	~ShaderProgram();
	void activate();

	void set_uniform_attribute(std::string attribute_name, const mat4& mat4);
	void set_vertex_attribute(std::string attribute_name, GLsizei stride, GLvoid* offset);
	static ShaderProgramPtr get_lines_program();
	static ShaderProgramPtr get_default_program();

private:
	GLuint _program_id;

	ShaderProgram();
	void attach_shaders(ShaderPtr vertex_shader, ShaderPtr fragment_shader);

	// disable
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram(ShaderProgram&& other) noexcept = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept = delete;
};

