#pragma once
#include "Model.h"
#include "RenderParameters.h"

class Light : public Model
{
public:
	Light();
	~Light();
	ModelType get_type() override { return LIGHT_MODEL; }
	void set_name(std::string name) override;
};
