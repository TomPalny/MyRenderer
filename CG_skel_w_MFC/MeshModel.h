#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include "Renderer.h"
#include "Model.h"
#include <string>

using namespace std;

class MeshModel : public Model
{
public:
	static std::vector<Face> load_faces(string filename);
	static Model* create_pyramid_model();

public:
	MeshModel(string filename);
	~MeshModel(void);
};
