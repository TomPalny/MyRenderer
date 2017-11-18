#pragma once
#include "vec.h"

struct Face
{
	Face() {}
	Face(vec4 p1, vec4 p2, vec4 p3) : point1(p1), point2(p2), point3(p3) {}

	vec4 point1, point2, point3;

	// I thought these would be useful for creating primitives 
	// like a cube but I think I was wrong
	Face copy_with_x_offset(const float offset) const;
	Face copy_with_y_offset(const float offset) const;
	Face copy_with_z_offset(const float offset) const;
};
