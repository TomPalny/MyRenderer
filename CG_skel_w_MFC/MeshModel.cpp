#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "MeshVAO.h"
#include "LinesVAO.h"

using namespace std;

struct FaceIdcs
{
	// TODO: why 4?
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec4 vec4fFromVec3Stream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec4(x, y, z, 1);
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

MeshModel::MeshModel(const string filename) : Model()
{
	auto faces = load_faces(filename);
	_vaos[VAO_MESH] = std::make_shared<MeshVAO>(faces, ShaderProgram::get_default_program());
	if (faces.size() > 0)
	{
		_vaos[VAO_BOUNDING_BOX] = LinesVAO::create_bounding_box_vao(faces);
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
	vector<vec4> normal_vertices;


	while (!ifile.eof() && !ifile.fail())
	{
		string cur_line;
		getline(ifile, cur_line);

		// read type of the line
		istringstream line_parser(cur_line);
		string line_type;
		line_parser >> std::ws >> line_type;

		// based on the type parse data
		if (line_type == "v") 
			vertices.push_back(vec4fFromVec3Stream(line_parser));
		else if (line_type == "vn")
		{
			auto normal = vec4fFromVec3Stream(line_parser);
			normal.w = 0;
			normal_vertices.push_back(normal);
		}
		else if (line_type == "f") 
			face_ids.push_back(line_parser);
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
			const auto normal1 = normal_vertices[face.vn[0] - 1];
			const auto normal2 = normal_vertices[face.vn[1] - 1];
			const auto normal3 = normal_vertices[face.vn[2] - 1];
			faces.push_back(Face(point1, point2, point3, normal1, normal2, normal3));
		}
	}
	return faces;
}
