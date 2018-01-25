#pragma once
#include <vector>
#include "VAO.h"
#include "Face.h"

class LinesVAO : public VAO
{
public:
	explicit LinesVAO(std::vector<vec4> lines);
	~LinesVAO();
	void draw() override;

	static VAOPtr create_bounding_box_vao(std::vector<Face>& faces, float& approximate_scale);
	static VAOPtr create_frustum_vao();
	static VAOPtr create_star_vao();

	// don't use these! they don't work with non-uniform scaling
	static VAOPtr create_vertex_normals_vao(std::vector<Face>& faces);
	static VAOPtr create_face_normals_vao(std::vector<Face>& faces);

protected:
	void setup_for_shader() override;

private:
	GLsizei _num_vertices;
};

