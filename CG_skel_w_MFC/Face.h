#pragma once
#include "vec.h"

struct Face
{
	Face(vec4 p1, vec4 p2, vec4 p3) : point1(p1), point2(p2), point3(p3), has_vertex_normals(false) {}
	Face(vec4 p1, vec4 p2, vec4 p3, vec4 n1, vec4 n2, vec4 n3) : Face(p1, p2, p3)
	{
		normal1 = n1;
		normal2 = n2;
		normal3 = n3;
		has_vertex_normals = true;
	}

	vec4 point1, point2, point3;
	vec4 normal1, normal2, normal3;
	bool has_vertex_normals = false;

	// I thought these would be useful for creating primitives 
	// like a cube but I think I was wrong
	Face copy_with_x_offset(const float offset) const;
	Face copy_with_y_offset(const float offset) const;
	Face copy_with_z_offset(const float offset) const;
};
