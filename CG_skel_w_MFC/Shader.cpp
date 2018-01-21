#include "stdafx.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <memory>
#include "GL/gl.h"
#include "GL/glew.h"

const std::string Shader::SHADER_PATH = "c:\\Projects\\technion\\graphics\\code\\shaders\\";

Shader::Shader(const std::string& filename, GLenum type)
{
	_name = filename;
	std::string path = SHADER_PATH + _name;
	std::string library_path = SHADER_PATH + "library.glsl";
	
	_id = glCreateShader(type);
	if (_id == 0)
	{
		throw std::exception("Could not create shader!");
	}
	
	std::string src = read_shader_source(path);
	std::string library = read_shader_source(library_path);

	const char* sources[] = { library.c_str(), src.c_str() };
	static const int TWO_SOURCE_STRINGS = 2;
	static const GLint* NUL_TERMINATED = NULL;
	glShaderSource(_id, TWO_SOURCE_STRINGS, sources, NUL_TERMINATED);
	glCompileShader(_id);

	GLint compiled;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		std::cerr << path << " failed to compile:" << std::endl;
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

GLuint Shader::get_id() const
{
	return _id;
}

std::string Shader::get_name() const
{
	return _name;
}


// Create a NULL-terminated string by reading the provided file
std::string Shader::read_shader_source(const std::string& filename)
{
	std::ifstream stream(filename);
	std::stringstream buffer;
	buffer << stream.rdbuf() << std::endl;
	return buffer.str();
}

