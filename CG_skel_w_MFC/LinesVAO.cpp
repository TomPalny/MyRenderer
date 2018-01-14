#include "stdafx.h"
#include "LinesVAO.h"
#include "Face.h"
#include <vector>

LinesVAO::LinesVAO(std::vector<vec4> points) : VAO(ShaderProgram::get_lines_program())
{
	std::vector<float> data;
	auto add_point = [&](auto point)
	{
		data.push_back(point.x);
		data.push_back(point.y);
		data.push_back(point.z);
		data.push_back(point.w);
	};
	for (auto p : points)
	{
		add_point(p);
	}

	static const int DIMENSIONS_PER_ATTRIBUTE = 4;
	_num_vertices = data.size() / DIMENSIONS_PER_ATTRIBUTE;

	glBindVertexArray(_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	static const int ZERO_STRIDE = 0;
	static GLvoid* ZERO_OFFSET = nullptr;
	_shader->set_vertex_attribute("vPosition", ZERO_STRIDE, ZERO_OFFSET);
	// TODO: ok we don't have vertex normals...
}

VAOPtr LinesVAO::create_bounding_box_vao(std::vector<Face>& faces)
{
	// flatten faces to vector of points
	std::vector<vec4> points;
	for (auto& face : faces)
	{
		points.push_back(face.point1);
		points.push_back(face.point2);
		points.push_back(face.point3);
	}

	// get min/max coordinates
	float min_x, min_y, min_z;
	float max_x = min_x = points[0].x;
	float max_y = min_y = points[0].y;
	float max_z = min_z = points[0].y;
	for (auto& point : points)
	{
		if (point.x > max_x) max_x = point.x;
		if (point.y > max_y) max_y = point.y;
		if (point.z > max_z) max_z = point.z;
		if (point.x < min_x) min_x = point.x;
		if (point.y < min_y) min_y = point.y;
		if (point.z < min_z) min_z = point.z;
	}

	// create array of line-data
	points.clear();
	auto add_line = [&](auto p1, auto p2)
	{
		points.push_back(p1);
		points.push_back(p2);
	};
	auto add_zface = [&](auto z)
	{
		add_line(vec4(min_x, min_y, z, 1), vec4(min_x, max_y, z, 1));
		add_line(vec4(max_x, min_y, z, 1), vec4(max_x, max_y, z, 1));
		add_line(vec4(min_x, min_y, z, 1), vec4(max_x, min_y, z, 1));
		add_line(vec4(min_x, max_y, z, 1), vec4(max_x, max_y, z, 1));
	};
	add_zface(min_z);
	add_zface(max_z);
	add_line(vec4(min_x, min_y, min_z, 1), vec4(min_x, min_y, max_z, 1));
	add_line(vec4(min_x, max_y, min_z, 1), vec4(min_x, max_y, max_z, 1));
	add_line(vec4(max_x, min_y, min_z, 1), vec4(max_x, min_y, max_z, 1));
	add_line(vec4(max_x, max_y, min_z, 1), vec4(max_x, max_y, max_z, 1));

	return VAOPtr(new LinesVAO(points));
}

VAOPtr LinesVAO::create_frustum_vao()
{
	// create array of line-data
	std::vector<vec4> points;

	static const float SIZE = 0.4f;
	float min_x=-SIZE, min_y=-SIZE, min_z=SIZE;
	float max_x= SIZE, max_y= SIZE, max_z=-SIZE*2;

	auto add_line = [&](auto p1, auto p2)
	{
		points.push_back(p1);
		points.push_back(p2);
	};
	auto add_zface = [&](auto z, auto scale)
	{
		add_line(vec4(min_x * scale, min_y * scale, z, 1), vec4(min_x * scale, max_y * scale, z, 1));
		add_line(vec4(max_x * scale, min_y * scale, z, 1), vec4(max_x * scale, max_y * scale, z, 1));
		add_line(vec4(min_x * scale, min_y * scale, z, 1), vec4(max_x * scale, min_y * scale, z, 1));
		add_line(vec4(min_x * scale, max_y * scale, z, 1), vec4(max_x * scale, max_y * scale, z, 1));
	};
	float scale = 2.0f;
	add_zface(min_z, 1.0f);
	add_zface(max_z, scale);
	add_line(vec4(min_x, min_y, min_z, 1), vec4(min_x * scale, min_y * scale, max_z, 1));
	add_line(vec4(min_x, max_y, min_z, 1), vec4(min_x * scale, max_y * scale, max_z, 1));
	add_line(vec4(max_x, min_y, min_z, 1), vec4(max_x * scale, min_y * scale, max_z, 1));
	add_line(vec4(max_x, max_y, min_z, 1), vec4(max_x * scale, max_y * scale, max_z, 1));

	return VAOPtr(new LinesVAO(points));
}

VAOPtr LinesVAO::create_star_vao()
{
	std::vector<vec4> points;
	auto add_line = [&](auto p1)
	{
		points.push_back(p1);
		auto p2 = -p1;
		p2.w = 1.0f;
		points.push_back(p2);
	};

	static const float LINE_LENGTH = 0.15f;
	add_line(vec4(LINE_LENGTH, 0, 0, 1));
	add_line(vec4(0, LINE_LENGTH, 0, 1));
	add_line(vec4(0, 0, LINE_LENGTH, 1));
	add_line(vec4(LINE_LENGTH, LINE_LENGTH, 0, 1));
	add_line(vec4(LINE_LENGTH, 0, LINE_LENGTH, 1));
	add_line(vec4(0, LINE_LENGTH, LINE_LENGTH, 1));
	add_line(vec4(LINE_LENGTH, LINE_LENGTH, LINE_LENGTH, 1));
	return VAOPtr(new LinesVAO(points));
}


LinesVAO::~LinesVAO()
{
}

void LinesVAO::draw()
{
	_shader->activate();
	glBindVertexArray(_vao_id);
	glDrawArrays(GL_LINES, 0, _num_vertices);
	glBindVertexArray(0);
}
