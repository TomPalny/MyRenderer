#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include "Renderer.h"
#include <string>

using namespace std;

class MeshModel : public Model
{
protected :
	MeshModel() {}
	vector<vec3> _vertex_positions;
	//add more attributes
	mat4 _world_transform;
	mat3 _normal_transform;
	Renderer* _renderer;

public:

	MeshModel(string fileName);
	~MeshModel(void);
	void load_file(string filename);
	void set_renderer(Renderer* renderer);

	// Inherited via Model
	virtual void draw() override;

};
