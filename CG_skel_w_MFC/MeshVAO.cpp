#include "stdafx.h"
#include "MeshVAO.h"
#include <vector>
#include "Face.h"


MeshVAO::MeshVAO(std::vector<Face>& faces, ShaderProgramPtr shader) : VAO(shader)
{
	std::vector<GLfloat> data;
	auto add_point = [&](auto point)
	{
		data.push_back(point.x);
		data.push_back(point.y);
		data.push_back(point.z);
		data.push_back(point.w);
	};

	for (auto& face : faces)
	{
		add_point(face.point1);
		add_point(face.normal1);
		add_point(face.point2);
		add_point(face.normal2);
		add_point(face.point3);
		add_point(face.normal3);
	}

	static const int DIMENSIONS_PER_ATTRIBUTE = 4;
	static const int ATTRIBUTES_PER_VERTEX = 2;
	static const int BYTES_BETWEEN_ATTRIBUTES = sizeof(float) * DIMENSIONS_PER_ATTRIBUTE * ATTRIBUTES_PER_VERTEX;
	_num_vertices = data.size() / (DIMENSIONS_PER_ATTRIBUTE * ATTRIBUTES_PER_VERTEX);

	glBindVertexArray(_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	static const int ZERO_OFFSET = 0;
	_shader->set_vertex_attribute("vPosition", BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)ZERO_OFFSET);
	_shader->set_vertex_attribute("vNormal", BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)(BYTES_BETWEEN_ATTRIBUTES / 2));
}

void MeshVAO::draw()
{
	_shader->activate();
	glBindVertexArray(_vao_id);
	glDrawArrays(GL_TRIANGLES, 0, _num_vertices);
}

MeshVAO::~MeshVAO()
{
}
