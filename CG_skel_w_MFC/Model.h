#pragma once
#include "Renderer.h"
#include "Face.h"

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
	Renderer* _renderer;
	mat4 _world_transforms;
	mat4 _model_transforms;
	mat4 _cached_world_model_transform;
	vector<Face> _faces;
	char* _name;
	char _origin_sign;
	
	

	vec4 transform_point(vec4 point) const;
	void draw_single_normal(vec4 start, vec4 direction);
	

public:
	virtual bool is_camera() { return false; }
	vec4 get_origin_in_world_coordinates();
	void update_matrix(mat4 view);
	void draw();
	void draw_vertex_normals();
	void draw_face_normals();
	void perform_operation(mat4 operation, TransformMode mode);
	void set_renderer(Renderer* renderer);
	const char* get_name() const;
	void set_name(string name);
	void create_bounding_box();
	float _min_x, _min_y, _min_z, _max_x, _max_y, _max_z;
	Model* _bounding_box;
};