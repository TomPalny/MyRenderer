#pragma once
#include "Face.h"
#include "mat.h"
#include <vector>
#include "RenderParameters.h"
#include <memory>
#include <string>
#include "Material.h"
#include "VAO.h"

class Renderer;

enum TransformMode
{
	WORLD_TRANSFORM,
	MODEL_TRANSFORM,
	NUMBER_OF_TRANSFORM_MODES
};

enum ModelType
{
	CAMERA_MODEL,
	LIGHT_MODEL,
	REGULAR_MODEL
};

enum VAOType
{
	VAO_MESH,
	VAO_BOUNDING_BOX
};

typedef std::unordered_map<VAOType, VAOPtr> VAOMap;

class Model {
protected:
	Model();
	virtual ~Model() {}

	mat4 _world_transforms;
	mat4 _model_transforms;
	char _origin_sign;
	std::string _name;
	VAOMap _vaos;

public:
	virtual ModelType get_type() { return REGULAR_MODEL; }
	vec4 get_origin_in_world_coordinates();
	mat4 get_transforms();

	void perform_operation(mat4 operation, TransformMode mode);
	std::string get_name() const;
	virtual void set_name(std::string name);
	void create_bounding_box();
	char get_origin_sign();

	// TODO: add VAO for bounding box, vertex normals, and face normals
	VAOMap& get_vaos();

	float _min_x, _min_y, _min_z, _max_x, _max_y, _max_z;
	Model* _bounding_box;
	MaterialPtr _material;
};