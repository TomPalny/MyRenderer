#include "stdafx.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <memory>
#include "GL/gl.h"
#include "GL/glew.h"

Shader::Shader(const std::string& filename, GLenum type)
{
	std::string full_path = "c:\\Projects\\technion\\graphics\\code\\shaders\\" + filename;

	_id = glCreateShader(type);
	if (_id == 0)
	{
		throw std::exception("Could not create shader!");
	}
	
	std::string src = read_shader_source(full_path);
	const char* sources[] = { src.c_str() };
	static const int ONE_SOURCE_STRING = 1;
	static const GLint* NUL_TERMINATED = NULL;
	glShaderSource(_id, ONE_SOURCE_STRING, sources, NUL_TERMINATED);
	glCompileShader(_id);

	GLint compiled;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << full_path << " failed to compile:" << std::endl;
		GLint log_size;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &log_size);
		std::shared_ptr<char> log_message(new char[log_size]);
		glGetShaderInfoLog(_id, log_size, NULL, log_message.get());
		std::cerr << std::string(log_message.get()) << std::endl;
		glDeleteShader(_id);
		throw std::exception("Shader Compilation Error!");
	}
}

Shader::~Shader()
{
	try
	{
		glDeleteShader(_id);
	}
	catch (...)
	{
		std::cout << "ERROR in ~Shader!" << std::endl;
	}
}

GLuint Shader::get_shader_id()
{
	return _id;
}


// Create a NULL-terminated string by reading the provided file
std::string Shader::read_shader_source(const std::string& filename)
{
	std::ifstream stream(filename);
	std::stringstream buffer;
	buffer << stream.rdbuf() << std::endl;
	return buffer.str();
}

