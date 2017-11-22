#include "stdafx.h"
#include "Camera.h"

Camera::Camera(vec3 transform)
{
	_transform = vec4(transform);
}


Camera::~Camera()
{
}

mat4 Camera::get_view_matrix()
{
	// what do world and model transforms even mean in this context?
	vec4 eye = _world_transforms * _model_transforms * vec4(0,0,0,1);
	vec4 at(0, 0, 0, 1);
	vec4 up(0, 1, 0, 0);

	vec4 n = normalize(eye - at);
	vec4 u = normalize(cross(up, n));
	vec4 v = normalize(cross(n, u));
	vec4 t = vec4(0, 0, 0, 1);
	mat4 c(u, v, n, t);
	return c * Translate(-eye);
}
