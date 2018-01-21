#pragma once
#include "Model.h"
#include "RenderParameters.h"

class Camera : public Model
{
	mat4 _view;
	ProjectionType _projection_type;
	float _fovy;
	float _aspect_ratio;
	float _nearz;
	float _farz;

public:
	Camera(int camera_id);
	~Camera();

	float get_aspect_ratio() const;
	void set_aspect_ratio(float aspect_ratio);
	ProjectionType get_projection_type() const;
	void set_projection_type(ProjectionType projection_type);
	float get_fovy() const;
	void set_fovy(float fovy);
	float get_nearz() const;
	void set_nearz(float nearz);
	float get_farz() const;
	void set_farz(float farz);
	ModelType get_type() override { return CAMERA_MODEL; }


	mat4 get_view_matrix();
	mat4 get_projection_matrix();

	void look_at(vec3 at);
	void look_at2(vec3 eye, vec3 at);
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
