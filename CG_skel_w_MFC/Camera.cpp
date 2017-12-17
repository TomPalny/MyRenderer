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
mat4 Camera::get_view_matrix(CameraMode camera_mode)
{
	if (camera_mode == ORTHOGONAL_CAMERA)
	{
		const float aspect_ratio = _renderer->get_width() / (float)_renderer->get_height();
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

		return ortho * _view;
	}
	else
	{
		float aspect_ratio = _renderer->get_width() / (float)_renderer->get_height();
		/* this is the version from the course slides:
		 *  https://webcourse.cs.technion.ac.il/234325/Winter2017-2018/ho/WCFiles/tut3.pdf
		float bottom = -0.5;
		float top = 0.5;
		float left = -0.5 * aspect_ratio;
		float right = 0.5 * aspect_ratio;
		float nearz = 0.5;
		float farz = 5;

		mat4 H;
		H[0][2] = (left + right) / (-2 * nearz);
		H[1][2] = (top + bottom) / (-2 * nearz);

		mat4 S;
		S[0][0] = (-2 * nearz) / (right - left);
		S[1][1] = (-2 * nearz) / (top - bottom);

		mat4 N;
		float alpha = -1 * (nearz + farz) / (nearz - farz);
		float beta = -2 * (nearz * farz) / (nearz - farz);
		N[2][2] = alpha;
		N[2][3] = beta;
		N[3][2] = -1;
		N[3][3] = 0;
		return N * S * H * _view;
		*/

		// this is the version from Interactive Computer Graphics version 6
		// there is a mistake in the book for gl_style[2][2]
		// we use the correct version here
		mat4 gl_style;
		float nearz = 0.5;
		float farz = 10;
		float top = nearz * tan(0.785398); // 45 degrees in radians
		float right = top * aspect_ratio;
		gl_style[0][0] = nearz / right;
		gl_style[1][1] = nearz / top;
		gl_style[2][2] = -1 * (farz + nearz) / (farz - nearz);
		gl_style[2][3] = -2*farz*nearz / (farz - nearz);
		gl_style[3][2] = -1;
		gl_style[3][3] = 0;
		return gl_style * _view;
	}
}

void Camera::apply_view_transformation(const mat4& inverse_operation)
{
	_view = _view * inverse_operation;
}
