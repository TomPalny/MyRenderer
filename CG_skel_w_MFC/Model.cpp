#include "stdafx.h"
#include "Model.h"
#include "mat.h"
#include "Renderer.h"

Model::Model() : _renderer(nullptr), _origin_sign('+')
{
}

void Model::set_renderer(Renderer * renderer)
{
	_renderer = renderer;
}

vec4 Model::transform_point(const vec4 point) const
{
	return _cached_world_model_transform * point;
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

void Model::create_bounding_box()
{
	_bounding_box = new Model();
	const vec4 point1(_min_x, _min_y, _min_z, 1);
	const vec4 point2(_min_x, _min_y, _max_z, 1);
	const vec4 point3(_max_x, _min_y, _max_z, 1);
	const vec4 point4(_max_x, _min_y, _min_z, 1);
	const vec4 point5(_min_x, _max_y, _min_z, 1);
	const vec4 point6(_min_x, _max_y, _max_z, 1);
	const vec4 point7(_max_x, _max_y, _max_z, 1);
	const vec4 point8(_max_x, _max_y, _min_z, 1);

	_bounding_box->_faces.push_back(Face(point1, point2, point3));
	_bounding_box->_faces.push_back(Face(point3, point4, point1));
	_bounding_box->_faces.push_back(Face(point5, point6, point7));
	_bounding_box->_faces.push_back(Face(point7, point8, point5));

	_bounding_box->_faces.push_back(Face(point6, point2, point3));
	_bounding_box->_faces.push_back(Face(point3, point7, point6));
	_bounding_box->_faces.push_back(Face(point7, point3, point4));
	_bounding_box->_faces.push_back(Face(point4, point8, point7));

	_bounding_box->_faces.push_back(Face(point8, point5, point1));
	_bounding_box->_faces.push_back(Face(point1, point4, point8));
	_bounding_box->_faces.push_back(Face(point5, point6, point2));
	_bounding_box->_faces.push_back(Face(point2, point1, point5));

}

void Model::draw(CameraMode camera_mode)
{
	_renderer->set_color(1, 1, 1);
	for (const auto face : _faces)
	{
		// TODO: we need to clip points that are out of range
		// for example, if points are behind us (negative z) then we 
		// currently draw them anyway which is totally wrong
		const auto point1 = transform_point(face.point1).to_vec3_divide_by_w();
		const auto point2 = transform_point(face.point2).to_vec3_divide_by_w();
		const auto point3 = transform_point(face.point3).to_vec3_divide_by_w();

		//_renderer->draw_point(point1);
		//_renderer->draw_point(point2);
		//_renderer->draw_point(point3);

		_renderer->draw_line_vcw(point1, point2, camera_mode);
		_renderer->draw_line_vcw(point2, point3, camera_mode);
		_renderer->draw_line_vcw(point3, point1, camera_mode);
	}

	vec4 origin = _cached_world_model_transform * vec4(0, 0, 0, 1);
	_renderer->set_color(1, 0, 0);
	_renderer->draw_letter_v(_origin_sign, origin.to_vec2());
}

void Model::draw_single_normal(vec4 start, vec4 direction, CameraMode camera_mode)
{
	_renderer->set_color(0.5, 0.5, 0.5);
	auto transformed_start = transform_point(start);
	auto end = start.to_vec3() + normalize(direction.to_vec3()) * 0.2f;
	auto transformed_end = transform_point(vec4(end));
	_renderer->draw_line_vcw(transformed_start.to_vec3_divide_by_w(), transformed_end.to_vec3_divide_by_w(), camera_mode);
}

vec4 Model::get_origin_in_world_coordinates()
{
	return _world_transforms * _model_transforms * vec4(0, 0, 0, 1);
}

void Model::update_matrix(mat4 view)
{
	_cached_world_model_transform = view * _world_transforms * _model_transforms;
}

void Model::draw_vertex_normals(CameraMode camera_mode)
{
	for (const auto face : _faces)
	{
		if (!face.has_vertex_normals)
		{
			continue;
		}
		draw_single_normal(face.point1, face.normal1, camera_mode);
		draw_single_normal(face.point2, face.normal2, camera_mode);
		draw_single_normal(face.point3, face.normal3, camera_mode);
	}
}

void Model::draw_face_normals(CameraMode camera_mode)
{
	for (const auto face : _faces)
	{
		auto vector1 = face.point2 - face.point1;
		auto vector2 = face.point3 - face.point2;
		auto normal = normalize(cross(vector1, vector2));
		auto center_of_triangle = (face.point1 + face.point2 + face.point3) / 3;
		draw_single_normal(center_of_triangle, normal, camera_mode);
	}
}

void Model::perform_operation(mat4 operation, TransformMode mode)
{
	if (mode == WORLD_TRANSFORM)
	{
		_world_transforms = operation * _world_transforms;
	}
	else
	{
		_model_transforms = operation * _model_transforms;
	}
}
