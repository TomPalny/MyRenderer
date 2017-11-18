#include "stdafx.h"
#include "Face.h"

Face Face::copy_with_x_offset(const float offset) const
{
	Face copy = *this;
	copy.point1.x += offset;
	copy.point2.x += offset;
	copy.point3.x += offset;
	return copy;
}

Face Face::copy_with_y_offset(const float offset) const
{
	Face copy = *this;
	copy.point1.y += offset;
	copy.point2.y += offset;
	copy.point3.y += offset;
	return copy;
}

Face Face::copy_with_z_offset(const float offset) const
{
	Face copy = *this;
	copy.point1.z += offset;
	copy.point2.z += offset;
	copy.point3.z += offset;
	return copy;
}