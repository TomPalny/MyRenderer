#include "stdafx.h"
#include "Model.h"
#include "mat.h"

Model::Model() : _renderer(nullptr)
{
}

void Model::translate(float x, float y, float z)
{
	// default translation
	mat4 translation;
	translation[0][3] = x; // x
	translation[1][3] = y; // y
	translation[2][3] = y; // z
	_model_to_world *= translation;
}

void Model::scale(float x, float y, float z)
{
	// default translation
	mat4 scale;
	scale[0][0] = x; // x
	scale[1][1] = y; // y
	scale[2][2] = y; // z
	_model_to_world *= scale;
}

void Model::set_renderer(Renderer * renderer)
{
	_renderer = renderer;
}

vec4 Model::transform_point(const vec4 point) const
{
	return _model_to_world * point;
}

vec2 Model::vec4_to_vec2(const vec4 point)
{
	return vec2(point.x / point.w, point.y / point.w);
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
