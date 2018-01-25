#pragma once
#include "VAO.h"
#include "Face.h"
#include <vector>
#include "Camera.h"

class MeshVAO : public VAO
{
public:
	void draw() override;
	~MeshVAO();

	static VAOPtr create_mesh_vao(std::vector<Face>& faces);
	static VAOPtr create_vertex_normals_vao(std::vector<Face>& faces);
	static VAOPtr create_face_normals_vao(std::vector<Face>& faces);
	static VAOPtr create_wireframe_vao(std::vector<Face>& faces);


protected:
	MeshVAO(std::vector<Face>& faces, ShaderProgramPtr shader);
	void setup_for_shader() override;

private:
	GLsizei _num_vertices;
};

