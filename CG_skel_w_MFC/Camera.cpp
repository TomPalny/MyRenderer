#include "stdafx.h"
#include "Camera.h"

Camera::Camera(vec3 transform)
{
	_transform = vec4(transform);
}


Camera::~Camera()
{
}

// at should be in the world coordinates
void Camera::look_at(vec4 at)
{
	// find our position in world coordinates
	vec4 eye = get_origin_in_world_coordinates();

	// these are also in world coordinates
	vec4 up(0, 1, 0, 0);

	// create a new view matrix that corresponds to this
	// we discard our old view matrix
	vec4 n = normalize(eye - at);
	vec4 u = normalize(cross(up, n));
	vec4 v = normalize(cross(n, u));
	vec4 t = vec4(0, 0, 0, 1);
	mat4 c(u, v, n, t);
	_view =  c * Translate(-eye);
}

mat4 Camera::get_view_matrix()
{
	return _view;
}
