#include "stdafx.h"
#include "Model.h"
#include "mat.h"
#include "Renderer.h"
#include "MeshVAO.h"
#include "LinesVAO.h"

Model::Model() : _origin_sign('+'), _material(Material::get_default()),
	_normal_type(NO_NORMALS), _fill_type(FILL_PHONG), _bounding_box(false), _uv_type(UV_BOX),
	_position_animation(POSITION_ANIMATION_DISABLED),_color_animation(COLOR_ANIMATION_DISABLED)
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

void Model::set_texture(TexturePtr& texture)
{
	_texture = texture;
}

TexturePtr Model::get_texture()
{
	return _texture;
}

char Model::get_origin_sign()
{
	return _origin_sign;
}

VAOMap& Model::get_vaos()
{
	return _vaos;
}

PositionAnimation Model::get_position_animation() const
{
	return _position_animation;
}

void Model::set_position_animation(PositionAnimation position_animation)
{
	_position_animation = position_animation;
}

ColorAnimation Model::get_color_animation() const
{
	return _color_animation;
}

void Model::set_color_animation(ColorAnimation color_animation)
{
	_color_animation = color_animation;
}

UVType Model::get_uv_type() const
{
	return _uv_type;
}

void Model::set_uv_type(UVType uv_type)
{
	_uv_type = uv_type;
}

NormalType Model::get_normal_type() const
{
	return _normal_type;
}

bool Model::is_bounding_box_enabled() const
{
	return _bounding_box;
}

FillType Model::get_fill_type() const
{
	return _fill_type;
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

void Model::set_normal_type(NormalType normal_type)
{
	_normal_type = normal_type;
}

void Model::set_bounding_box(bool bounding_box)
{
	_bounding_box = bounding_box;
}

void Model::set_fill_type(FillType fill_type)
{
	_fill_type = fill_type;
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
