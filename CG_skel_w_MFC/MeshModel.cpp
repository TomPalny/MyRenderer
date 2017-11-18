#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

MeshModel::MeshModel(string filename)
{
	load_file(filename);
}

MeshModel::~MeshModel(void)
{
}

void MeshModel::load_file(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> face_ids;
	vector<vec4> vertices;
	vector<vec4> normal_vertices;
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") 
			vertices.push_back(vec4fFromVec3Stream(issLine));
		else if (lineType == "vn")
			normal_vertices.push_back(vec4fFromVec3Stream(issLine));
		else if (lineType == "f") 
			face_ids.push_back(issLine);
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line / vn
		}
		else
		{
			cout<< "Found unknown line Type \"" << lineType << "\"";
		}
	}
	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertex_positions should contain:
	//vertex_positions={v1,v2,v3,v1,v3,v4}

	_faces.clear();
	// iterate through all stored faces and create triangles
	for (const auto face : face_ids)
	{
		auto point1 = vertices[face.v[0] - 1];
		auto point2 = vertices[face.v[1] - 1];
		auto point3 = vertices[face.v[2] - 1];
		// if some of the vertices don't have a normal than disable normals for this face
		if (face.vn[0] == 0 || face.vn[1] == 0 || face.vn[2] == 0)
		{
			_faces.push_back(Face(point1, point2, point3));
		}
		else
		{
			auto normal1 = normal_vertices[face.vn[0] - 1];
			auto normal2 = normal_vertices[face.vn[1] - 1];
			auto normal3 = normal_vertices[face.vn[2] - 1];
			_faces.push_back(Face(point1, point2, point3, normal1, normal2, normal3));
		}
	}
}
