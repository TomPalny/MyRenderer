#include "stdafx.h"
#include "Model.h"
#include "mat.h"
#include "Renderer.h"
#include "MeshVAO.h"
#include "LinesVAO.h"

Model::Model() : _origin_sign('+'), _bounding_box(nullptr), _material(Material::get_default())
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

char Model::get_origin_sign()
{
	return _origin_sign;
}

VAOMap& Model::get_vaos()
{
	return _vaos;
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
