#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MeshVAO.h"
#include "LinesVAO.h"
#include "Serialization.h"
#include "Serialization.h"

using namespace std;

MeshModel::MeshModel(const string filename) : Model()
{
	auto faces = load_faces(filename);
	_vaos[VAO_MESH] = MeshVAO::create_mesh_vao(faces);
	_vaos[VAO_BOUNDING_BOX] = LinesVAO::create_bounding_box_vao(faces);
	_vaos[VAO_FACE_NORMALS] = MeshVAO::create_face_normals_vao(faces);
	if (faces[0].has_vertex_normals)
	{
		_vaos[VAO_VERTEX_NORMALS] = MeshVAO::create_vertex_normals_vao(faces);
	}
}

MeshModel::~MeshModel(void)
{
}

std::vector<Face> MeshModel::load_faces(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> face_ids;
	vector<vec4> vertices;
	vector<vec4> normals;
	vector<vec2> uv_coordinates;

	while (!ifile.eof())
	{
		string cur_line;
		getline(ifile, cur_line);

		// read type of the line
		istringstream line_parser(cur_line);
		string line_type;
		line_parser >> std::ws >> line_type;

		// based on the type parse data
		if (line_type == "v")
		{
			vertices.push_back(vec4fFromVec3Stream(line_parser));
		}
		else if (line_type == "vn")
		{
			auto normal = vec4fFromVec3Stream(line_parser);
			normal.w = 0;
			normals.push_back(normal);
		}
		else if (line_type == "vt")
		{
			auto uv = vec2fFromStream(line_parser);
			uv_coordinates.push_back(uv);
		}
		else if (line_type == "f")
		{
			face_ids.push_back(FaceIdcs(line_parser));
		}
		else if (line_type == "#" || line_type == "")
		{
			// comment / empty line / vn
		}
		else
		{
			cout << "Found unknown line Type \"" << line_type << "\"";
		}
	}

	std::vector<Face> faces;
	// iterate through all stored faces and create triangles
	for (const auto face : face_ids)
	{
		const auto point1 = vertices[face.v[0] - 1];
		const auto point2 = vertices[face.v[1] - 1];
		const auto point3 = vertices[face.v[2] - 1];
		
		// if some of the vertices don't have a normal than disable normals for this face
		if (face.vn[0] == 0 || face.vn[1] == 0 || face.vn[2] == 0)
		{
			faces.push_back(Face(point1, point2, point3));
		}
		else
		{
			const auto normal1 = normals[face.vn[0] - 1];
			const auto normal2 = normals[face.vn[1] - 1];
			const auto normal3 = normals[face.vn[2] - 1];
			faces.push_back(Face(point1, point2, point3, normal1, normal2, normal3));
		}
		if (face.vt[0] != 0 && face.vt[1] != 0 && face.vt[2] != 0)
		{
			const auto uv1 = uv_coordinates[face.vt[0] - 1];
			const auto uv2 = uv_coordinates[face.vt[1] - 1];
			const auto uv3 = uv_coordinates[face.vt[2] - 1];
			faces.rbegin()->set_uv(uv1, uv2, uv3);
		}
	}
	std::cout << "Face count: " << faces.size() << std::endl;
	return faces;
}

Model* MeshModel::create_pyramid_model()
{
	return new MeshModel("c:\\Projects\\technion\\graphics\\code\\models\\pyramid.obj");
}
