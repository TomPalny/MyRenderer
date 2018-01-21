#pragma once
#include "vec.h"

struct Face
{
	Face(vec4 p1, vec4 p2, vec4 p3);
	Face(vec4 p1, vec4 p2, vec4 p3, vec4 n1, vec4 n2, vec4 n3);
	void set_uv(vec2 p1, vec2 p2, vec2 p3);
	vec4 point1, point2, point3;
	vec4 normal1, normal2, normal3, face_normal;
	vec2 uv1, uv2, uv3;
	bool has_vertex_normals;
	bool has_uv_coordinates;
};
