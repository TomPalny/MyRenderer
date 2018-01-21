#pragma once
#include <unordered_map>

enum ProjectionType
{
	ORTHOGONAL_PROJECTION,
	PERSPECTIVE_PROJECTION,
	NUMBER_OF_PROJECTION_TYPES
};

enum NormalType
{
	NO_NORMALS,
	VERTEX_NORMALS,
	FACE_NORMALS,
	NUMBER_OF_NORMAL_TYPES
};

static const std::unordered_map<NormalType, std::string> NORMAL_TYPE_MAP
{
	{NO_NORMALS, "NONE"}, 
	{VERTEX_NORMALS, "VERTEX"},
	{FACE_NORMALS, "FACE"}
};

enum FillType
{
	FILL_FLAT,
	FILL_GOURAUD,
	FILL_PHONG,
	NUMBER_OF_FILL_TYPES
};

static const std::unordered_map<FillType, std::string> FILL_TYPE_MAP
{
	{ FILL_FLAT, "FLAT" },
	{ FILL_GOURAUD, "GOURAUD" },
	{ FILL_PHONG, "PHONG" }
};

enum UVType
{
	UV_PREDEFINED,
	UV_BOX,
	UV_SPHERE,
	NUMBER_OF_UV_TYPES
};

static const std::unordered_map<UVType, std::string> UV_TYPE_MAP
{
	{ UV_PREDEFINED, "PREDEFINED (FILE)" },
	{ UV_BOX, "BOX" },
	{ UV_SPHERE, "SPHERE" }
};

enum PositionAnimation
{
	POSITION_ANIMATION_DISABLED,
	POSITION_ANIMATION1,
	POSITION_ANIMATION2
};

static const std::unordered_map<PositionAnimation, std::string> POSITION_ANIMATION_MAP
{
	{ POSITION_ANIMATION_DISABLED, "DISABLED" },
	{ POSITION_ANIMATION1, "ANIMATION1" },
	{ POSITION_ANIMATION2, "ANIMATION2" }
};

enum ColorAnimation
{
	COLOR_ANIMATION_DISABLED,
	COLOR_ANIMATION1,
	COLOR_ANIMATION2
};

static const std::unordered_map<ColorAnimation, std::string> COLOR_ANIMATION_MAP
{
	{ COLOR_ANIMATION_DISABLED, "DISABLED" },
	{ COLOR_ANIMATION1, "ANIMATION1" },
	{ COLOR_ANIMATION2, "ANIMATION2" }
};

enum OperationMode
{
	TRANSLATE_MODE,
	ROTATE_MODE,
	SCALE_MODE
};

