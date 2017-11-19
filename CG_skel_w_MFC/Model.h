#pragma once
#include "Renderer.h"

struct Face
{
	vec4 point1, point2, point3;
};

class Model {
protected:
	virtual ~Model() {}
	mat4 _world_transform;
	mat3 _normal_transform;
	Renderer* _renderer;
	mat4 _model_to_world;
	vector<Face> _faces;
	char* _name;

public:
	void draw();
	void translate(float x, float y, float z);
	void rotate(float theta, char axis);
	void scale(float x, float y, float z);
	void set_renderer(Renderer* renderer);
	vec2 transform_point(vec4 point) const;
	const char* get_name();
	void set_name(string name);
};