#pragma once
#include "Model.h"
#include "RenderParameters.h"

class Light : public Model
{
public:
	Light(int light_id);
	~Light();
	ModelType get_type() override { return LIGHT_MODEL; }
};
