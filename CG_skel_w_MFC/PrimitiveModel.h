#pragma once
#include "Model.h"
class PrimitiveModel : public Model
{
	PrimitiveModel(std::vector<Face> faces);
public:
	~PrimitiveModel();
	static PrimitiveModel* create_pyramid(float center_x, float center_y, float center_z);
};

