#include "stdafx.h"
#include "Camera.h"
#include "Renderer.h"

Camera::Camera(int camera_id)
{
	_origin_sign = '0' + camera_id;
}


Camera::~Camera()
{
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

mat4 Camera::get_projection_matrix(CameraMode camera_mode, float aspect_ratio)
{
	if (camera_mode == ORTHOGONAL_CAMERA)
	{
		const float bottom = -5;
		const float top = 5;
		const float left = -5 * aspect_ratio;
		const float right = 5 * aspect_ratio;
		const float nearz = 0.1;
		float farz = 10;
		
		mat4 ortho;
		ortho[0][0] = 2 / (right - left);
		ortho[1][1] = 2 / (top - bottom);
		ortho[2][2] = -2 / (farz - nearz);
		ortho[3][3] = 1;

		ortho[0][3] = -(left + right) / (right - left);
		ortho[1][3] = -(top + bottom) / (top - bottom);
		ortho[2][3] = -(farz + nearz) / (farz - nearz);

		return ortho;
	}
	else
	{
		// this is from Interactive Computer Graphics version 6
		// there is a mistake in the book for gl_style[2][2]
		// we use the correct version here
		mat4 persp;
		float nearz = 0.5;
		float farz = 10;
		float top = nearz * tan(0.785398); // 45 degrees in radians
		float right = top * aspect_ratio;
		persp[0][0] = nearz / right;
		persp[1][1] = nearz / top;
		persp[2][2] = -1 * (farz + nearz) / (farz - nearz);
		persp[2][3] = -2*farz*nearz / (farz - nearz);
		persp[3][2] = -1;
		persp[3][3] = 0;
		return persp;
	}
}

void Camera::apply_view_transformation(const mat4& inverse_operation)
{
	_view = _view * inverse_operation;
}
