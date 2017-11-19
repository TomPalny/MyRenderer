#include "stdafx.h"
#include "Model.h"
#include "mat.h"

void Model::translate(float x, float y, float z)
{
	// default translation
	mat4 translation;
	translation[0][3] = x; // x
	translation[1][3] = y; // y
	translation[2][3] = y; // z
	_model_to_world *= translation;
}

void Model::scale(const float x, const float y, const float z)
{
	mat4 scale;
	scale[0][0] = x; // x
	scale[1][1] = y; // y
	scale[2][2] = z; // z
	_model_to_world *= scale;
}


void Model::set_renderer(Renderer * renderer)
{
	_renderer = renderer;
}

vec2 Model::transform_point(const vec4 point) const
{
	const vec4 translated_point = _model_to_world * point;
	return vec2(translated_point.x / translated_point.w, translated_point.y / translated_point.w);
}

const char* Model::get_name()
{
	return _name;
}

void Model::set_name(string name)
{
	char *cstr = new char[name.length() + 1];
	strcpy(cstr, name.c_str());
	_name = cstr;
}


void Model::draw()
{
	for (auto face : _faces)
	{
		auto point1 = transform_point(face.point1);
		auto point2 = transform_point(face.point2);
		auto point3 = transform_point(face.point3);

		//_renderer->draw_point(point1);
		//_renderer->draw_point(point2);
		//_renderer->draw_point(point3);

		_renderer->draw_line(point1, point2);
		_renderer->draw_line(point2, point3);
		_renderer->draw_line(point3, point1);
	}
}
