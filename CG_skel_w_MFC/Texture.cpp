#include "stdafx.h"
#include "Texture.h"
#include "lodepng.h"


Texture::Texture(const std::string& filename, int texture_unit) :
	_texture_unit(texture_unit)
{
	glGenTextures(1, &_texture_id);

	unsigned width, height;
	unsigned error = lodepng::decode(_buffer, width, height, filename);
	if (error != 0)
	{
		glDeleteTextures(1, &_texture_id);
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		throw std::exception("Could not load image!");
	}

	glBindTexture(GL_TEXTURE_2D, _texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	static const int LOD_0 = 0;
	static const int RESERVED = 0;
	glTexImage2D(GL_TEXTURE_2D, LOD_0, GL_RGBA, width, height, RESERVED, GL_RGBA, GL_UNSIGNED_BYTE, _buffer.data());
}

void Texture::activate()
{
	glActiveTexture(GL_TEXTURE0 + _texture_unit);
	glBindTexture(GL_TEXTURE_2D, _texture_id);
}

Texture::~Texture()
{
	try
	{
		glDeleteTextures(1, &_texture_id);
	}
	catch (...)
	{
		std::cout << "Caught exception in ~Texture!" << std::endl;
	}
}
