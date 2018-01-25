#include "stdafx.h"
#include "Texture.h"
#include "lodepng.h"


Texture::Texture(int texture_unit, int type) :
	_texture_unit(texture_unit), _type(type)
{
	glGenTextures(1, &_texture_id);
}

void Texture::add_image(const std::string& filename, const int subtype, bool flipped)
{
	unsigned width, height;
	std::vector<unsigned char> buffer;
	unsigned error = lodepng::decode(buffer, width, height, filename);
	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		throw std::exception("Could not load image!");
	}

	glBindTexture(_type, _texture_id);
	if (flipped)
	{
		glTexParameteri(_type, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(_type, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	}
	else
	{
		glTexParameteri(_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameteri(_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	static const int LOD_0 = 0;
	static const int RESERVED = 0;
	glTexImage2D(subtype, LOD_0, GL_RGBA, width, height, RESERVED, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
}

void Texture::activate()
{
	glActiveTexture(GL_TEXTURE0 + _texture_unit);
	glBindTexture(_type, _texture_id);
}

TexturePtr Texture::load_texture(const std::string& filename)
{
	TexturePtr texture(new Texture(PRIMARY_TEXTURE_UNIT, GL_TEXTURE_2D));
	texture->add_image(filename, GL_TEXTURE_2D, false);
	return texture;
}

TexturePtr Texture::load_bumpmap(const std::string& filename)
{
	TexturePtr texture(new Texture(BUMP_MAPPING_TEXTURE_UNIT, GL_TEXTURE_2D));
	texture->add_image(filename, GL_TEXTURE_2D, false);
	return texture;
}

TexturePtr Texture::load_noise()
{
	TexturePtr texture(new Texture(PRIMARY_TEXTURE_UNIT, GL_TEXTURE_2D));
	texture->add_image("c:\\Projects\\technion\\graphics\\code\\textures\\noiselevel1.png", GL_TEXTURE_2D, true);
	return texture;
}

TexturePtr Texture::load_cubemap(const std::string& filename_prefix)
{
	TexturePtr texture(new Texture(BUMP_MAPPING_TEXTURE_UNIT, GL_TEXTURE_CUBE_MAP));
	try
	{
		texture->add_image(filename_prefix + "_up.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y, false);
		texture->add_image(filename_prefix + "_dn.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, false);
		texture->add_image(filename_prefix + "_rt.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z, false);
		texture->add_image(filename_prefix + "_lf.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, false);
		texture->add_image(filename_prefix + "_ft.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X, false);
		texture->add_image(filename_prefix + "_bk.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X, false);
	}
	catch(...)
	{
		texture->add_image(filename_prefix + "posy.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y, false);
		texture->add_image(filename_prefix + "negy.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, false);
		texture->add_image(filename_prefix + "posz.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z, false);
		texture->add_image(filename_prefix + "negz.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, false);
		texture->add_image(filename_prefix + "posx.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X, false);
		texture->add_image(filename_prefix + "negx.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X, false);
	}
	return texture;
}

bool Texture::is_cubemap()
{
	return _type == GL_TEXTURE_CUBE_MAP;
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
