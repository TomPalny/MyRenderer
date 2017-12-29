#include "stdafx.h"
#include "Light.h"
#include "Scene.h"

Light::Light()
{
	static int light_count = 0;
	light_count++;

	_origin_sign = '*';
	_material = Material::get_default_white_light();
	char light_char = '0' + light_count;
	char light_str[2] = { light_char, '\0' };
	std::string name = std::string("Light ") + std::string(light_str);
	Model::set_name(name);
}


Light::~Light()
{
}

void Light::set_name(std::string name)
{
	// do nothing - this light already has a name
}
