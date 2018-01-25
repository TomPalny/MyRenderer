#pragma once
#include "Face.h"
#include "mat.h"
#include <vector>
#include "RenderParameters.h"
#include <memory>
#include <string>
#include "Material.h"
#include "VAO.h"
#include "Texture.h"

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
	VAO_BOUNDING_BOX,
	VAO_VERTEX_NORMALS,
	VAO_FACE_NORMALS,
	VAO_WIREFRAME,
	VAO_TEST
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
	TexturePtr _texture;
	TexturePtr _bump_texture;
	NormalType _normal_type;
	bool _bounding_box;
	bool _toon_shading;
	bool _marble_texture;
	bool _skybox;
	bool _environment_mapping;
	bool _wireframe;
	FillType _fill_type;
	UVType _uv_type;
	PositionAnimation _position_animation;
	ColorAnimation _color_animation;

public:
	bool is_wireframe_enabled() const;
	void set_wireframe_enabled(bool wireframe);
	bool is_environment_mapping() const;
	void set_environment_mapping(bool environment_mapping);
	bool is_skybox() const;
	void set_skybox(bool skybox);
	TexturePtr get_bump_texture() const;
	void set_bump_texture(const TexturePtr& bump_texture);
	bool is_marble_texture_enabled() const;
	void set_marble_texture(bool marble_texture);
	bool is_toon_shading_enabled() const;
	void set_toon_shading(bool toon_shading);
	PositionAnimation get_position_animation() const;
	void set_position_animation(PositionAnimation position_animation);
	ColorAnimation get_color_animation() const;
	void set_color_animation(ColorAnimation color_animation);
	UVType get_uv_type() const;
	void set_uv_type(UVType uv_type);
	NormalType get_normal_type() const;
	bool is_bounding_box_enabled() const;
	FillType get_fill_type() const;
	virtual ModelType get_type() { return REGULAR_MODEL; }
	vec4 get_origin_in_world_coordinates();
	mat4 get_transforms();

	void set_normal_type(NormalType normal_type);
	void set_bounding_box(bool bounding_box);
	void set_fill_type(FillType fill_type);
	void perform_operation(mat4 operation, TransformMode mode);
	std::string get_name() const;
	virtual void set_name(std::string name);
	void set_texture(TexturePtr& texture);
	TexturePtr get_texture();
	char get_origin_sign();

	VAOMap& get_vaos();
	MaterialPtr _material;
};