#pragma once
#include "Renderer.h"
#include "Face.h"

class Model {
protected:
	virtual ~Model() {}
	mat4 _world_transform;
	mat3 _normal_transform;
	Renderer* _renderer;
	mat4 _model_to_world;
	vector<Face> _faces;

	vec4 transform_point(vec4 point) const;
	static vec2 vec4_to_vec2(const vec4 point);
	void draw_single_normal(vec4 start, vec4 direction);

public:
	Model();
	void draw();
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void set_renderer(Renderer* renderer);
	void draw_vertex_normals();
};