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

public:
	void draw();
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void set_renderer(Renderer* renderer);
	vec2 transform_point(vec4 point) const;
};