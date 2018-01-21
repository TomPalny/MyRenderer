#include "stdafx.h"
#include "VAO.h"

VAO::VAO(ShaderProgramPtr shader) : _shader(shader)
{
	glGenVertexArrays(1, &_vao_id);
	glGenBuffers(1, &_buffer_id);
}

VAO::~VAO()
{
	try
	{
		glDeleteVertexArrays(1, &_vao_id);
		glDeleteBuffers(1, &_buffer_id);
	}
	catch (...)
	{
		std::cerr << "ERROR: Exception in ~VAO" << std::endl;
	}
}

ShaderProgramPtr VAO::get_shader_program()
{
	return _shader;
}

void VAO::set_shader_program(ShaderProgramPtr shader_program)
{
	_shader = shader_program;
	setup_for_shader();
}

void VAO::reload_shader()
{
	try
	{
		_shader = _shader->reload_shader();
		setup_for_shader();
	}
	catch (...)
	{
		MessageBoxA(NULL, "Shader Error!", "Oops", 0);
	}
}
