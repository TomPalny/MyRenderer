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

	static VAOPtr create_bounding_box_vao(std::vector<Face>& faces);
	static VAOPtr create_frustum_vao();
	static VAOPtr create_star_vao();
private:
	GLsizei _num_vertices;
};

