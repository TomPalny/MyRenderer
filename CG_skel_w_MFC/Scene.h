#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
using namespace std;

class Model {
protected:
	virtual ~Model() {}

public:
	void virtual draw()=0;
};


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

class Scene {

	vector<Model*> _models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *_renderer;

public:
	Scene() {};
	Scene(Renderer *renderer) : _renderer(renderer) {};
	void load_obj_model(string fileName);
	void draw();
	void draw_demo();
	
	int activeModel;
	int activeLight;
	int activeCamera;
};