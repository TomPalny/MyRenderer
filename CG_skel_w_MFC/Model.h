#pragma once
#include "Renderer.h"
#include "Face.h"

class Model {
protected:
	Model();
	virtual ~Model() {}
	mat4 _world_transform;
	mat3 _normal_transform;
	Renderer* _renderer;
	mat4 _model_to_world;
	vector<Face> _faces;
	char* _name;

	vec4 transform_point(vec4 point) const;
	vec2 vec4_to_vec2(const vec4 point);
	void draw_single_normal(vec4 start, vec4 direction);

public:
	void draw();
	void draw_vertex_normals();
	void draw_face_normals();
	void translate(float x, float y, float z);
	void rotate(float theta, char axis);
	void scale(float x, float y, float z);
	void set_renderer(Renderer* renderer);
	const char* get_name() const;
	void set_name(string name);
};