#pragma once
#include "Model.h"
#include "RenderParameters.h"

typedef enum
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	NUM_LIGHT_TYPES
} LightType;

class Light : public Model
{
public:
	Light();
	~Light();
	ModelType get_type() override { return LIGHT_MODEL; }
	void set_name(std::string name) override;

	LightType light_type;
};
