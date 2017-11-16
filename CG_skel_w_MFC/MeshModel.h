#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include "Renderer.h"
#include <string>

using namespace std;

struct Face
{
	vec4 point1, point2, point3;
};

class MeshModel : public Model
{
protected :
	MeshModel() {}
	//add more attributes
	mat4 _world_transform;
	mat3 _normal_transform;
	Renderer* _renderer;
	mat4 _model_to_world;
	vector<Face> _faces;

public:

	MeshModel(string fileName);
	~MeshModel(void);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);
	void load_file(string filename);
	void set_renderer(Renderer* renderer);
	vec2 transform_point(vec4 point) const;

	// Inherited via Model
	virtual void draw() override;

};
