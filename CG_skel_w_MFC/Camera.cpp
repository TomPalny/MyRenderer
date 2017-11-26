#include "stdafx.h"
#include "Camera.h"

Camera::Camera(int camera_id)
{
	_origin_sign = '0' + camera_id;
}


Camera::~Camera()
{
}

// at should be in the world coordinates
void Camera::look_at(vec4 at)
{
	// find our position in world coordinates
	vec3 eye = get_origin_in_world_coordinates().to_vec3();

	// these are also in world coordinates
	vec3 up(0, 1, 0);

	// create a new view matrix that corresponds to this
	// we discard our old view matrix
	vec3 n = normalize(eye - at.to_vec3());
	vec3 u = normalize(cross(up, n));
	vec3 v = normalize(cross(n, u));
	vec4 t = vec4(0, 0, 0, 1);
	mat4 c(vec4(u, 0), vec4(v, 0), vec4(n, 0), t);
	
	_view =  c * Translate(-eye);
}

mat4 Camera::get_view_matrix()
{
	const float aspect_ratio = _renderer->get_width() / (float)_renderer->get_height();
	const float bottom = -5;
	const float top = 5;
	const float left = -5 * aspect_ratio;	
	const float right = 5 * aspect_ratio;
	const float nearz = -5;
	const float farz = 5;

	mat4 ortho;
	ortho[0][0] = 2 / (right - left);
	ortho[1][1] = 2 / (top - bottom);
	ortho[2][2] = -2 / (farz - nearz);
	ortho[3][3] = 1;

	ortho[0][3] = -(left+right)/(right-left);
	ortho[1][3] = -(top+bottom)/(top-bottom);
	ortho[2][3] = -(farz+nearz)/(farz-nearz);

	return ortho * _view;
}

void Camera::apply_view_transformation(const mat4& inverse_operation)
{
	_view = _view * inverse_operation;
}
