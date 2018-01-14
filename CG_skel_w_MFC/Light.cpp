#include "stdafx.h"
#include "Light.h"
#include "Scene.h"
#include "LinesVAO.h"

Light::Light() : Model()
{
	_vaos[VAO_MESH] = LinesVAO::create_star_vao();

	light_type = POINT_LIGHT;

	static int light_count = 0;
	light_count++;

	_origin_sign = '*';
	_material = Material::get_default_white_light();
	char light_char = '0' + light_count;
	char light_str[2] = { light_char, '\0' };
	std::string name = std::string("Light ") + std::string(light_str);
	Model::set_name(name);

	// TODO: the size is wrong on this
	/*const vec4 point1(0.1, 0.1, 0.1, 1);
	const vec4 point2(-0.1, -0.1, 0.1, 1);
	const vec4 point3(-0.1, 0.1, -0.1, 1);
	_faces->push_back(Face(point3, point2, point1));*/
}


Light::~Light()
{
}

void Light::set_name(std::string name)
{
	// do nothing - this light already has a name
}
