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
	/*const vec4 point1(0.3, 0.3, 0.3, 1);
	const vec4 point2(-0.3, -0.3, 0.3, 1);
	const vec4 point3(-0.3, 0.3,  -0.3, 1);
	const vec4 point4(0.3, -0.3, -0.3, 1);

	PrimitiveModel* model = new PrimitiveModel();
	model->_faces->push_back(Face(point3, point2, point1));
	model->_faces->push_back(Face(point1, point2, point4));
	model->_faces->push_back(Face(point4, point3, point1));
	model->_faces->push_back(Face(point2, point3, point4));*/

	const vec4 point1(0.0, 0.6, 0.3, 1);
	const vec4 point2(-0.3, -0.3, -0.15, 1);
	const vec4 point3(0.3, -0.3,  -0.15, 1);

	PrimitiveModel* model = new PrimitiveModel();
	model->_faces->push_back(Face(point1, point2, point3));
	return model;
}
