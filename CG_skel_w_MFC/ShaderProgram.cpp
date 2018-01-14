#include "stdafx.h"
#include "ShaderProgram.h"
#include <sstream>
#include "Shader.h"
#include "mat.h"


ShaderProgram::ShaderProgram()
{
	_program_id = glCreateProgram();
	if (_program_id == 0)
	{
		throw std::exception("Could not create shader");
	}
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::activate()
{
	glUseProgram(_program_id);
}

void ShaderProgram::attach_shaders(ShaderPtr vertex_shader, ShaderPtr fragment_shader)
{
	glAttachShader(_program_id, vertex_shader->get_shader_id());
	glAttachShader(_program_id, fragment_shader->get_shader_id());

	GLint linked;
	glLinkProgram(_program_id);
	glGetProgramiv(_program_id, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cerr << "Shader program failed to link" << std::endl;
		GLint log_size;
		glGetProgramiv(_program_id, GL_INFO_LOG_LENGTH, &log_size);
		std::shared_ptr<char> log_message(new char[log_size]);
		glGetProgramInfoLog(_program_id, log_size, NULL, log_message.get());
		std::cerr << std::string(log_message.get()) << std::endl;
		throw std::exception("Error in ShaderProgram::attach_shaders!");
	}
}

// this assumes that the VAO and buffer are already bound
void ShaderProgram::set_vertex_attribute(std::string attribute_name, GLsizei stride, GLvoid* offset)
{
	// I'm not sure why this is necessary but it is
	activate();

	static const int FOUR_COMPONENTS = 4;
	static const GLboolean DONT_NORMALIZE = GL_FALSE;
	GLuint loc = glGetAttribLocation(_program_id, attribute_name.c_str());
	if (loc == -1)
	{
		std::cout << "WARNING: No such vertex attribute: " << attribute_name << std::endl;
		return;
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, FOUR_COMPONENTS, GL_FLOAT, DONT_NORMALIZE, stride, offset);
}

void ShaderProgram::set_uniform_attribute(std::string attribute_name, const vec4& vec)
{
	// I'm not sure why this is necessary but it is
	activate();

	GLuint loc = glGetUniformLocation(_program_id, attribute_name.c_str());
	if (loc == -1)
	{
		//std::cout << "WARNING: No such uniform attribute: " << attribute_name << std::endl;
		return;
	}

	static const GLsizei ONE_MATRIX = 1;
	static const GLboolean TRANSPOSE_TRUE = GL_TRUE;
	glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::set_uniform_attribute(std::string attribute_name, const mat4& mat4)
{
	// I'm not sure why this is necessary but it is
	activate();

	GLuint loc = glGetUniformLocation(_program_id, attribute_name.c_str());
	if (loc == -1)
	{
		//std::cout << "WARNING: No such uniform attribute: " << attribute_name << std::endl;
		return;
	}

	GLfloat data[16] = { 0 };
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			data[row * 4 + col] = mat4[row][col];
		}
	}

	static const GLsizei ONE_MATRIX = 1;
	static const GLboolean TRANSPOSE_TRUE = GL_TRUE;
	glUniformMatrix4fv(loc, ONE_MATRIX, TRANSPOSE_TRUE, data);
}

ShaderProgramPtr ShaderProgram::get_default_program()
{
	static ShaderProgramPtr default_program(NULL);
	if (default_program != nullptr)
	{
		return default_program;
	}

	auto vertex_shader = std::make_shared<Shader>("minimal_vshader.glsl", GL_VERTEX_SHADER);
	auto fragment_shader = std::make_shared<Shader>("minimal_fshader.glsl", GL_FRAGMENT_SHADER);
	default_program = ShaderProgramPtr(new ShaderProgram());
	default_program->attach_shaders(vertex_shader, fragment_shader);
	return default_program;
};

ShaderProgramPtr ShaderProgram::get_lines_program()
{
	static ShaderProgramPtr program(NULL);
	if (program != nullptr)
	{
		return program;
	}

	auto vertex_shader = std::make_shared<Shader>("minimal_vshader.glsl", GL_VERTEX_SHADER);
	auto fragment_shader = std::make_shared<Shader>("lines_fshader.glsl", GL_FRAGMENT_SHADER);
	program = ShaderProgramPtr(new ShaderProgram());
	program->attach_shaders(vertex_shader, fragment_shader);
	return program;
}
