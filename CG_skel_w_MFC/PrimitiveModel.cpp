#include "stdafx.h"
#include "PrimitiveModel.h"


PrimitiveModel::PrimitiveModel()
{
	_bounding_box = nullptr;
}


PrimitiveModel::~PrimitiveModel()
{
}

PrimitiveModel* PrimitiveModel::create_pyramid(float center_x, float center_y, float center_z)
{
	const vec4 point1(1, 1, 1, 1);
	const vec4 point2(-1, -1, 1, 1);
	const vec4 point3(-1, 1,  -1, 1);
	const vec4 point4(1, -1, -1, 1);

	PrimitiveModel* model = new PrimitiveModel();
	//model->_faces->push_back(Face(point1, point2, point3));
	//model->_faces->push_back(Face(point1, point2, point4));
	//model->_faces->push_back(Face(point1, point3, point4));
	model->_faces->push_back(Face(point2, point3, point4));
	return model;
}
