#include "stdafx.h"
#include "Model.h"
#include "mat.h"
#include "Renderer.h"

Model::Model() :  _faces(new std::vector<Face>()), _origin_sign('+'), _bounding_box(nullptr), _material(Material::get_default())
{
}

std::string Model::get_name() const
{
	return _name;
}

void Model::set_name(string name)
{
	_name = name;
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

	_bounding_box->_faces->push_back(Face(point1, point2, point3));
	_bounding_box->_faces->push_back(Face(point3, point4, point1));
	_bounding_box->_faces->push_back(Face(point5, point6, point7));
	_bounding_box->_faces->push_back(Face(point7, point8, point5));

	_bounding_box->_faces->push_back(Face(point6, point2, point3));
	_bounding_box->_faces->push_back(Face(point3, point7, point6));
	_bounding_box->_faces->push_back(Face(point7, point3, point4));
	_bounding_box->_faces->push_back(Face(point4, point8, point7));

	_bounding_box->_faces->push_back(Face(point8, point5, point1));
	_bounding_box->_faces->push_back(Face(point1, point4, point8));
	_bounding_box->_faces->push_back(Face(point5, point6, point2));
	_bounding_box->_faces->push_back(Face(point2, point1, point5));

}

std::shared_ptr<std::vector<Face> > Model::get_faces()
{
	return _faces;
}

char Model::get_origin_sign()
{
	return _origin_sign;
}

vec4 Model::get_origin_in_world_coordinates()
{
	// we exclude _model_transforms
	// the origin is always at (0,0,0) in the model frame
	// when you move a point in the model frame you are moving it 
	// relative to the origin which stays at (0,0,0)
	return _world_transforms * vec4(0, 0, 0, 1);
}

mat4 Model::get_transforms()
{
	return _world_transforms * _model_transforms;
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