#include "stdafx.h"
#include "PrimitiveModel.h"


PrimitiveModel::PrimitiveModel()
{
}


PrimitiveModel::~PrimitiveModel()
{
}

PrimitiveModel* PrimitiveModel::create_pyramid(float center_x, float center_y, float center_z)
{
	const vec4 point1(1, 0, -1 / sqrt(2), 1);
	const vec4 point2(-1, 0, -1 / sqrt(2), 1);
	const vec4 point3(0, 1,  1 / sqrt(2), 1);
	const vec4 point4(0, 1, 1 / sqrt(2), 1);

	PrimitiveModel* model = new PrimitiveModel();
	model->_faces.push_back(Face(point1, point2, point3));
	model->_faces.push_back(Face(point1, point3, point4));
	model->_faces.push_back(Face(point2, point3, point4));
	return model;
}
