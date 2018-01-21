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

	void set_vertex_attribute(std::string attribute_name, int components, GLsizei stride, GLvoid* offset);
	void set_uniform_attributef(std::string attribute_name, const float value);
	void set_uniform_attributei(std::string attribute_name, const int value);
	void set_uniform_attribute(std::string attribute_name, const vec4& vec);
	void set_uniform_attribute(std::string attribute_name, const mat4& mat4);
	std::string get_vertex_shader_path();
	std::string get_fragment_shader_path();

	ShaderProgramPtr reload_shader(); // returns a newly initialized copy of the current shader
	static ShaderProgramPtr get_lines_program();
	static ShaderProgramPtr get_vertex_normals_program();
	static ShaderProgramPtr get_face_normals_program();
	static ShaderProgramPtr get_default_program();
	static ShaderProgramPtr create(std::string vertex_shader_path, std::string geometry_path, std::string fragment_shader_path);

private:
	GLuint _program_id;
	std::string _vertex_path;
	std::string _geometry_path;
	std::string _fragment_path;

	ShaderProgram();
	void attach_shaders(ShaderPtr vertex_shader, ShaderPtr geometry_shader_path, ShaderPtr fragment_shader);

	// disable
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram(ShaderProgram&& other) noexcept = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept = delete;
};

