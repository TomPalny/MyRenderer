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
