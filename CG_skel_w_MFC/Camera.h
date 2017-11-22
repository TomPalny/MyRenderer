#pragma once
#include "Model.h"

class Camera : public Model {
	vec4 _transform;
	mat4 _projection;

public:
	explicit Camera(vec3 transform);
	~Camera();

	mat4 get_view_matrix();

	/*
	void SetTransformation(const mat4& transform);
	void LookAt(const vec4& eye, const vec4& at, const vec4& up);
	void Ortho(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);
	void Frustum(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);
	mat4 Perspective(const float fovy, const float aspect,
		const float zNear, const float zFar);*/

};