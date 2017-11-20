#include "stdafx.h"
#include "Model.h"
#include "mat.h"

Model::Model() : _renderer(nullptr)
{
}

void Model::translate(float x, float y, float z, TransformMode mode)
{
	// default translation
	mat4 translation;
	translation[0][3] = x; // x
	translation[1][3] = y; // y
	translation[2][3] = z; // z
	if (mode == WORLD_TRANSFORM)
	{
		_world_transforms *= translation;
	}
	else
	{
		_model_transforms *= translation;
	}
}

void Model::rotate(float theta, char axis, TransformMode mode)
{
	mat4 rotation;
	switch (axis)
	{
	case 'x':
		rotation[0][0] = 1;
		rotation[1][1] = cos(theta);
		rotation[1][2] = sin(theta);
		rotation[2][1] = -sin(theta);
		rotation[2][2] = cos(theta);
		rotation[3][3] = 1;
		break;
	case 'y':
		rotation[0][0] = cos(theta);
		rotation[0][2] = -sin(theta);
		rotation[1][1] = 1;
		rotation[2][0] = sin(theta);
		rotation[2][2] = cos(theta);
		rotation[3][3] = 1;
		break;
	case 'z':
		rotation[0][0] = cos(theta);
		rotation[0][1] = sin(theta);
		rotation[1][0] = -sin(theta);
		rotation[1][1] = cos(theta);
		rotation[2][2] = 1;
		rotation[3][3] = 1;
		break;
	}

	if (mode == WORLD_TRANSFORM)
	{
		_world_transforms *= rotation;
	}
	else
	{
		_model_transforms *= rotation;
	}
}


void Model::scale(const float x, const float y, const float z, TransformMode mode)
{
	mat4 scale;
	scale[0][0] = x; // x
	scale[1][1] = y; // y
	scale[2][2] = z; // z

	if (mode == WORLD_TRANSFORM)
	{
		_world_transforms *= scale;
	}
	else
	{
		_model_transforms *= scale;
	}
}

void Model::set_renderer(Renderer * renderer)
{
	_renderer = renderer;
}

vec4 Model::transform_point(const vec4 point) const
{
	return _world_transforms * _model_transforms * point;
}

vec2 Model::vec4_to_vec2(const vec4 point)
{
	return vec2(point.x / point.w, point.y / point.w);
}
const char* Model::get_name() const
{
	return _name;
}

void Model::set_name(string name)
{
	// TODO: free this
	char *cstr = new char[name.length() + 1];
	strcpy(cstr, name.c_str());
	_name = cstr;
}

void Model::draw()
{
	for (const auto face : _faces)
	{
		const auto point1 = vec4_to_vec2(transform_point(face.point1));
		const auto point2 = vec4_to_vec2(transform_point(face.point2));
		const auto point3 = vec4_to_vec2(transform_point(face.point3));

		//_renderer->draw_point(point1);
		//_renderer->draw_point(point2);
		//_renderer->draw_point(point3);

		_renderer->draw_line(point1, point2);
		_renderer->draw_line(point2, point3);
		_renderer->draw_line(point3, point1);
	}
}

void Model::draw_single_normal(vec4 start, vec4 direction)
{
	auto transformed_start = transform_point(start);
	auto end = start + direction;
	end.w = 1;
	auto transformed_end = transform_point(end);
	_renderer->draw_line(vec4_to_vec2(transformed_start), vec4_to_vec2(transformed_end));
}

void Model::draw_vertex_normals()
{
	for (const auto face : _faces)
	{
		if (!face.has_vertex_normals)
		{
			continue;
		}
		draw_single_normal(face.point1, face.normal1);
		draw_single_normal(face.point2, face.normal2);
		draw_single_normal(face.point3, face.normal3);
	}
}

void Model::draw_face_normals()
{
	for (const auto face : _faces)
	{
		auto vector1 = face.point2 - face.point1;
		auto vector2 = face.point3 - face.point2;
		auto normal = normalize(cross(vector1, vector2));
		auto center_of_triangle = (face.point1 + face.point2 + face.point3) / 3;
		draw_single_normal(center_of_triangle, normal);
	}
}
