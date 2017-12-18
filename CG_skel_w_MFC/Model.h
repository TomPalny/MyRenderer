#pragma once
#include "Face.h"
#include "mat.h"
#include <vector>
#include "CameraMode.h"
#include <memory>

class Renderer;

enum TransformMode
{
	WORLD_TRANSFORM,
	MODEL_TRANSFORM,
	NUMBER_OF_TRANSFORM_MODES
};

class Model {
protected:
	Model();
	virtual ~Model() {}
	mat4 _world_transforms;
	mat4 _model_transforms;
	std::shared_ptr<std::vector<Face> > _faces;
	char* _name;
	char _origin_sign;

	// TODO: what should we do with these?
	// TODO: should the Model call the renderer?
	// that might let us leave some higher level logic (e.g. about normals) outside of the renderer
	// I think that makes sense... All transformations are done outside of the renderer and in the model?
	// I'm not going to do it that way now but I can refactor it later
	void draw_single_normal(::vec4 start, ::vec4 direction, CameraMode camera_mode);
	void draw_vertex_normals(CameraMode camera_mode);
	void draw_face_normals(CameraMode camera_mode);

public:
	virtual bool is_camera() { return false; }
	vec4 get_origin_in_world_coordinates();
	mat4 get_transforms();

	void perform_operation(mat4 operation, TransformMode mode);
	const char* get_name() const;
	void set_name(std::string name);
	void create_bounding_box();
	std::shared_ptr<std::vector<Face> > get_faces();
	char get_origin_sign();
	float _min_x, _min_y, _min_z, _max_x, _max_y, _max_z;
	Model* _bounding_box;
};