#pragma once
#include "ShaderProgram.h"

class VAO
{
public:
	virtual ~VAO();
	virtual void draw() = 0;
	ShaderProgramPtr get_shader_program();

protected:
	GLuint _vao_id;
	GLuint _buffer_id;
	ShaderProgramPtr _shader;

	explicit VAO(ShaderProgramPtr shader);

public:
	// disable
	VAO(const VAO& other) = delete;
	VAO(VAO&& other) noexcept = delete;
	VAO& operator=(const VAO& other) = delete;
	VAO& operator=(VAO&& other) noexcept = delete;
};

typedef std::shared_ptr<VAO> VAOPtr;