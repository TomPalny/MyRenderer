#pragma once
#include "ShaderProgram.h"

class VAO
{
public:
	virtual ~VAO();
	virtual void draw() = 0;
	ShaderProgramPtr get_shader_program();
	void set_shader_program(ShaderProgramPtr shader_program);
	void reload_shader();

protected:
	GLuint _vao_id;
	GLuint _buffer_id;
	ShaderProgramPtr _shader;

	explicit VAO(ShaderProgramPtr shader);
	virtual void setup_for_shader() = 0;

public:
	// disable
	VAO(const VAO& other) = delete;
	VAO(VAO&& other) noexcept = delete;
	VAO& operator=(const VAO& other) = delete;
	VAO& operator=(VAO&& other) noexcept = delete;
};

typedef std::shared_ptr<VAO> VAOPtr;