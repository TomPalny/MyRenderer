#pragma once
#include "VAO.h"
#include "Face.h"
#include <vector>

class MeshVAO : public VAO
{
public:
	MeshVAO(std::vector<Face>& faces, ShaderProgramPtr shader);
	void draw() override;
	~MeshVAO();

private:
	GLsizei _num_vertices;
};

