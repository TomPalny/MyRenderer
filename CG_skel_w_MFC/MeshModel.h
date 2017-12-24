#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include "Renderer.h"
#include "Model.h"
#include <string>

using namespace std;

// TODO: this should really be renamed to FileModel
class MeshModel : public Model
{
protected :
	MeshModel() {}
	
	void load_file(string filename);

public:
	MeshModel(string filename);
	~MeshModel(void);
};
