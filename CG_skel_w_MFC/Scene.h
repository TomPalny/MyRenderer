#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "Model.h"

using namespace std;

class Light {

};

class Camera {
	mat4 cTransform;
	mat4 projection;

public:
	void setTransformation(const mat4& transform);
	void LookAt(const vec4& eye, const vec4& at, const vec4& up );
	void Ortho( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	void Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );
	mat4 Perspective( const float fovy, const float aspect,
		const float zNear, const float zFar);

};

enum NormalType
{
	NO_NORMALS,
	VERTEX_NORMALS,
	FACE_NORMALS,
	NUMBER_OF_NORMAL_TYPES
};

class Scene {
	Model* _active_model;
	vector<Model*> _models;
	vector<Light*> _lights;
	vector<Camera*> _cameras;
	Renderer *_renderer;
	NormalType _normal_type;

public:
	explicit Scene(Renderer* renderer);;

	void add_pyramid_model();
	void load_model_at_center(Model* model, const string name);
	void load_obj_model(string filename, string name);

	void draw();
	void draw_demo() const;
	void switch_active_model(int id);
	static void redraw_necessary();
	void add_objects_to_menu();

	void keyboard(unsigned char key, int x, int y);
	void keyboard_special(int key, int x, int y);

	int activeModel;
	int activeLight;
	int activeCamera;
	bool scale_flag = false;
	bool rotate_flag = false;
};