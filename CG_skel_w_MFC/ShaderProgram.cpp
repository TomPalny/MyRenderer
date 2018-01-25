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

void ShaderProgram::attach_shaders(ShaderPtr vertex_shader, ShaderPtr geometry_shader, ShaderPtr fragment_shader)
{
	_vertex_path = vertex_shader->get_name();
	_fragment_path = fragment_shader->get_name();
	if (geometry_shader == nullptr)
	{
		_geometry_path = "";
	}
	else
	{
		_geometry_path = geometry_shader->get_name();
	}

	glAttachShader(_program_id, vertex_shader->get_id());
	if (geometry_shader != nullptr)
	{
		glAttachShader(_program_id, geometry_shader->get_id());
	}
	glAttachShader(_program_id, fragment_shader->get_id());

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
void ShaderProgram::set_vertex_attribute(std::string attribute_name, int components, GLsizei stride, GLvoid* offset)
{
	activate();
	static const GLboolean DONT_NORMALIZE = GL_FALSE;
	GLuint loc = glGetAttribLocation(_program_id, attribute_name.c_str());
	if (loc == -1)
	{
		std::cout << "WARNING: No such vertex attribute: " << attribute_name << std::endl;
		return;
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, components, GL_FLOAT, DONT_NORMALIZE, stride, offset);
}

void ShaderProgram::set_uniform_attributef(std::string attribute_name, const float value)
{
	activate();

	GLuint loc = glGetUniformLocation(_program_id, attribute_name.c_str());
	if (loc == -1)
	{
		//std::cout << "WARNING: No such uniform attribute: " << attribute_name << std::endl;
		return;
	}

	glUniform1f(loc, value);
}

void ShaderProgram::set_uniform_attributei(std::string attribute_name, const int value)
{
	activate();

	GLuint loc = glGetUniformLocation(_program_id, attribute_name.c_str());
	if (loc == -1)
	{
		//std::cout << "WARNING: No such uniform attribute: " << attribute_name << std::endl;
		return;
	}

	glUniform1i(loc, value);
}

void ShaderProgram::set_uniform_attribute(std::string attribute_name, const vec4& vec)
{
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

std::string ShaderProgram::get_vertex_shader_path()
{
	return _vertex_path;
}

std::string ShaderProgram::get_fragment_shader_path()
{
	return _fragment_path;
}

// reloading one of the default shaders only reloads it for a specific object
// future objects will still be created using the old version returned by e.g. get_default_program
// TODO: fix that
ShaderProgramPtr ShaderProgram::reload_shader()
{
	return create(_vertex_path, _geometry_path, _fragment_path);
}

ShaderProgramPtr ShaderProgram::create(std::string vertex_shader_path, std::string geometry_shader_path, std::string fragment_shader_path)
{
	ShaderPtr vertex_shader = std::make_shared<Shader>(vertex_shader_path, GL_VERTEX_SHADER);
	ShaderPtr fragment_shader = std::make_shared<Shader>(fragment_shader_path, GL_FRAGMENT_SHADER);
	ShaderPtr geometry_shader = nullptr;
	if (geometry_shader_path != "")
	{
		geometry_shader = std::make_shared<Shader>(geometry_shader_path, GL_GEOMETRY_SHADER);
	}
	auto program = ShaderProgramPtr(new ShaderProgram());
	program->attach_shaders(vertex_shader, geometry_shader, fragment_shader);
	return program;
}

ShaderProgramPtr ShaderProgram::get_default_program()
{
	static ShaderProgramPtr program(NULL);
	if (program != nullptr)
	{
		return program;
	}

	program = create("vshader_standard.glsl", "", "fshader_standard.glsl");
	return program;
};

ShaderProgramPtr ShaderProgram::get_lines_program()
{
	static ShaderProgramPtr program(NULL);
	if (program != nullptr)
	{
		return program;
	}

	program = create("vshader_standard.glsl", "", "fshader_white.glsl");
	return program;
}

ShaderProgramPtr ShaderProgram::get_vertex_normals_program()
{
	static ShaderProgramPtr program(NULL);
	if (program != nullptr)
	{
		return program;
	}

	program = create("vshader_standard.glsl", "gshader_vertex_normals.glsl", "fshader_white.glsl");
	return program;
}

ShaderProgramPtr ShaderProgram::get_face_normals_program()
{
	static ShaderProgramPtr program(NULL);
	if (program != nullptr)
	{
		return program;
	}

	program = create("vshader_standard.glsl", "gshader_face_normals.glsl", "fshader_white.glsl");
	return program;
}

ShaderProgramPtr ShaderProgram::get_wireframe_program()
{
	static ShaderProgramPtr program(NULL);
	if (program != nullptr)
	{
		return program;
	}

	program = create("vshader_standard.glsl", "gshader_wireframe.glsl", "fshader_white.glsl");
	return program;
}
