#include "stdafx.h"
#include "MeshVAO.h"
#include <vector>
#include "Face.h"

static const int FLOATS_FOR_POSITION = 4;
static const int FLOATS_FOR_NORMAL = 4;
static const int FLOATS_FOR_UV = 2;
static const int FLOATS_FOR_TANGENT = 3;
static const int FLOATS_FOR_BITANGENT = 3;
static const int TOTAL_NUM_FLOATS = FLOATS_FOR_POSITION + FLOATS_FOR_NORMAL + FLOATS_FOR_UV + FLOATS_FOR_TANGENT + FLOATS_FOR_BITANGENT;
static const int BYTES_BETWEEN_ATTRIBUTES = sizeof(float) * TOTAL_NUM_FLOATS;

MeshVAO::MeshVAO(std::vector<Face>& faces, ShaderProgramPtr shader) : VAO(shader)
{
	std::vector<GLfloat> data;
	auto add_vec4 = [&](auto point)
	{
		data.push_back(point.x);
		data.push_back(point.y);
		data.push_back(point.z);
		data.push_back(point.w);
	};
	auto add_vec3 = [&](auto point)
	{
		data.push_back(point.x);
		data.push_back(point.y);
		data.push_back(point.z);
	};
	auto add_vec2 = [&](auto point)
	{
		data.push_back(point.x);
		data.push_back(point.y);
	};
	for (auto& face : faces)
	{
		vec3 delta_pos1 = (face.point2 - face.point1).to_vec3();
		vec3 delta_pos2 = (face.point3 - face.point1).to_vec3();
		vec2 delta_uv1 = face.uv2 - face.uv1;
		vec2 delta_uv2 = face.uv3 - face.uv1;
		float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
		vec3 tangent = (delta_pos1 * delta_uv2.y - delta_pos2 * delta_uv1.y) * r;
		vec3 bitangent = (delta_pos2 * delta_uv1.x - delta_pos1 * delta_uv2.x) * r;
		tangent = normalize(tangent - face.face_normal.to_vec3() * dot(face.face_normal.to_vec3(), tangent));
		if (dot(cross(face.face_normal.to_vec3(), tangent), bitangent) < 0.0f) {
			tangent = tangent * -1.0f;
		}

		add_vec4(face.point1);
		add_vec4(face.normal1);
		add_vec2(face.uv1);
		add_vec3(tangent);
		add_vec3(bitangent);

		add_vec4(face.point2);
		add_vec4(face.normal2);
		add_vec2(face.uv2);
		add_vec3(tangent);
		add_vec3(bitangent);

		add_vec4(face.point3);
		add_vec4(face.normal3);
		add_vec2(face.uv3);
		add_vec3(tangent);
		add_vec3(bitangent);
	}

	_num_vertices = data.size() / TOTAL_NUM_FLOATS;
	std::cout << "_num_vertices: " << _num_vertices << std::endl;

	glBindVertexArray(_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

	MeshVAO::setup_for_shader();
}

void MeshVAO::setup_for_shader()
{
	glBindVertexArray(_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, _buffer_id);

	static const int ZERO_OFFSET = 0;
	static const int FOUR_COMPONENTS = 4;
	static const int THREE_COMPONENTS = 3;
	static const int TWO_COMPONENTS = 2;
	_shader->set_vertex_attribute("vPosition", FOUR_COMPONENTS, BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)ZERO_OFFSET);
	_shader->set_vertex_attribute("vNormal", FOUR_COMPONENTS, BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)(FLOATS_FOR_POSITION * sizeof(float)));
	_shader->set_vertex_attribute("vUV", TWO_COMPONENTS, BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)((FLOATS_FOR_POSITION + FLOATS_FOR_NORMAL) * sizeof(float)));
	_shader->set_vertex_attribute("vTangent", THREE_COMPONENTS, BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)((FLOATS_FOR_POSITION + FLOATS_FOR_NORMAL + FLOATS_FOR_UV) * sizeof(float)));
	_shader->set_vertex_attribute("vBitangent", THREE_COMPONENTS, BYTES_BETWEEN_ATTRIBUTES, (GLvoid*)((FLOATS_FOR_POSITION + FLOATS_FOR_NORMAL + FLOATS_FOR_UV + FLOATS_FOR_TANGENT) * sizeof(float)));
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

VAOPtr MeshVAO::create_mesh_vao(std::vector<Face>& faces)
{
	return VAOPtr(new MeshVAO(faces, ShaderProgram::get_default_program()));
}

VAOPtr MeshVAO::create_vertex_normals_vao(std::vector<Face>& faces)
{
	return VAOPtr(new MeshVAO(faces, ShaderProgram::get_vertex_normals_program()));
}

VAOPtr MeshVAO::create_face_normals_vao(std::vector<Face>& faces)
{
	return VAOPtr(new MeshVAO(faces, ShaderProgram::get_face_normals_program()));
}

VAOPtr MeshVAO::create_wireframe_vao(std::vector<Face>& faces)
{
	return VAOPtr(new MeshVAO(faces, ShaderProgram::get_wireframe_program()));
}
