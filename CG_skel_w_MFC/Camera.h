#pragma once
#include "Model.h"
#include "RenderParameters.h"

class Camera : public Model
{
	mat4 _view;
	ProjectionType _projection_type;
	float _fovy;
	float _aspect_ratio;

public:
	ModelType get_type() override { return CAMERA_MODEL; }

	Camera(int camera_id);
	~Camera();

	mat4 get_view_matrix();
	mat4 get_projection_matrix();

	void look_at(vec3 at);
	void look_at2(vec3 eye, vec3 at);
	void set_camera_parameters(ProjectionType projection_type, float aspect_ratio, float fovy);
	void apply_view_transformation(const mat4& inverse_operation);

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
