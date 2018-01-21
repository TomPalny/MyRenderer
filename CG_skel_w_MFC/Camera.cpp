#include "stdafx.h"
#include "Camera.h"
#include "Renderer.h"
#include "RenderParameters.h"
#include "MeshModel.h"
#include "LinesVAO.h"
#include "MeshVAO.h"

Camera::Camera(int camera_id) : _nearz(0.9), _farz(40), _fovy(35)
{
	_origin_sign = '0' + camera_id;
	_vaos[VAO_MESH] = LinesVAO::create_frustum_vao();
	//perform_operation(Scale(0.3f, 0.3f, 0.3f), MODEL_TRANSFORM);
}

Camera::~Camera()
{
}

float Camera::get_aspect_ratio() const
{
	return _aspect_ratio;
}

void Camera::set_aspect_ratio(float aspect_ratio)
{
	_aspect_ratio = aspect_ratio;
}

ProjectionType Camera::get_projection_type() const
{
	return _projection_type;
}

void Camera::set_projection_type(ProjectionType projection_type)
{
	_projection_type = projection_type;
}

float Camera::get_fovy() const
{
	return _fovy;
}

void Camera::set_fovy(float fovy)
{
	_fovy = fovy;
}

float Camera::get_nearz() const
{
	return _nearz;
}

void Camera::set_nearz(float nearz)
{
	_nearz = nearz;
}

float Camera::get_farz() const
{
	return _farz;
}

void Camera::set_farz(float farz)
{
	_farz = farz;
}

// at should be in the world coordinates
void Camera::look_at(vec3 at)
{
	// find our position in world coordinates
	vec3 eye = get_origin_in_world_coordinates().to_vec3();

	// these are also in world coordinates
	vec3 up(0, 1, 0);

	// create a new view matrix that corresponds to this
	// we discard our old view matrix
	vec3 n = normalize(eye - at);
	vec3 u = normalize(cross(up, n));
	vec3 v = normalize(cross(n, u));
	vec4 t = vec4(0, 0, 0, 1);
	mat4 c(vec4(u, 0), vec4(v, 0), vec4(n, 0), t);
	
	_view =  c * Translate(-eye);
}

// at should be in the world coordinates
void Camera::look_at2(vec3 eye, vec3 at)
{
	// these are also in world coordinates
	vec3 up(0, 1, 0);

	// create a new view matrix that corresponds to this
	// we discard our old view matrix
	vec3 n = normalize(eye - at);
	vec3 u = normalize(cross(up, n));
	vec3 v = normalize(cross(n, u));
	vec4 t = vec4(0, 0, 0, 1);
	mat4 c(vec4(u, 0), vec4(v, 0), vec4(n, 0), t);

	_view = c * Translate(-eye);
}

mat4 Camera::get_view_matrix()
{
	return _view;
}

mat4 Camera::get_projection_matrix()
{
	mat4 mat;

	if (_projection_type == ORTHOGONAL_PROJECTION)
	{
		const float bottom = -2;
		const float top = 2;
		const float left = -2 * _aspect_ratio;
		const float right = 2 * _aspect_ratio;
		
		mat[0][0] = 2 / (right - left);
		mat[1][1] = 2 / (top - bottom);
		mat[2][2] = -2 / (_farz - _nearz);
		mat[3][3] = 1;

		mat[0][3] = -(left + right) / (right - left);
		mat[1][3] = -(top + bottom) / (top - bottom);
		mat[2][3] = -(_farz + _nearz) / (_farz - _nearz);
	}
	else
	{
		// this is from Interactive Computer Graphics version 6
		// there is a mistake in the book for persp[2][2]
		// we use the correct version here
		float top = _nearz * tan(_fovy * M_PI / 180.0f); // convert degrees to radians
		float right = top * _aspect_ratio;
		mat[0][0] = _nearz / right;
		mat[1][1] = _nearz / top;
		mat[2][2] = -1 * (_farz + _nearz) / (_farz - _nearz);
		mat[2][3] = -2*_farz*_nearz / (_farz - _nearz);
		mat[3][2] = -1;
		mat[3][3] = 0;
	}

	glDepthRangef(_nearz, _farz);
	return mat;
}

void Camera::apply_view_transformation(const mat4& inverse_operation)
{
	_view = _view * inverse_operation;
}
