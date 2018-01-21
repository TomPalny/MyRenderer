#include "stdafx.h"
#include "Face.h"

Face::Face(vec4 p1, vec4 p2, vec4 p3): point1(p1), point2(p2), point3(p3), has_vertex_normals(false), has_uv_coordinates(false)
{
	auto vector1 = (p2 - p1).to_vec3();
	auto vector2 = (p3 - p2).to_vec3();
	face_normal = vec4(normalize(cross(vector1, vector2)), 0);
	normal1 = normal2 = normal3 = face_normal;
}

Face::Face(vec4 p1, vec4 p2, vec4 p3, vec4 n1, vec4 n2, vec4 n3): Face(p1, p2, p3)
{
	normal1 = n1;
	normal2 = n2;
	normal3 = n3;
	has_vertex_normals = true;
}

void Face::set_uv(vec2 p1, vec2 p2, vec2 p3)
{
	uv1 = p1;
	uv2 = p2;
	uv3 = p3;
	has_uv_coordinates = true;
}
