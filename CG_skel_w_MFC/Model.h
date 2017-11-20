#pragma once
#include "Renderer.h"
#include "Face.h"

enum TransformMode
{
	WORLD_TRANSFORM,
	MODEL_TRANSFORM
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

	vec4 transform_point(vec4 point) const;
	vec2 vec4_to_vec2(const vec4 point);
	void draw_single_normal(vec4 start, vec4 direction);

public:
	void update_matrix();
	void draw();
	void draw_vertex_normals();
	void draw_face_normals();
	void translate(float x, float y, float z, TransformMode mode);
	void rotate(float theta, char axis, TransformMode mode);
	void scale(float x, float y, float z, TransformMode mode);
	void set_renderer(Renderer* renderer);
	const char* get_name() const;
	void set_name(string name);
};