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

void MeshModel::translate(float x, float y, float z)
{
	// default translation
	mat4 translation;
	translation[0][3] = x; // x
	translation[1][3] = y; // y
	translation[2][3] = y; // z
	_model_to_world *= translation;
}

void MeshModel::scale(float x, float y, float z)
{
	// default translation
	mat4 scale;
	scale[0][0] = x; // x
	scale[1][1] = y; // y
	scale[2][2] = y; // z
	_model_to_world *= scale;
}

void MeshModel::load_file(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> face_ids;
	vector<vec4> vertices;
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
		else if (lineType == "f") 
			face_ids.push_back(issLine);
		else if (lineType == "#" || lineType == "" || lineType == "vn")
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
	for (const auto it : face_ids)
	{
		Face face;
		face.point1 = vertices[it.v[0] - 1];
		face.point2 = vertices[it.v[1] - 1];
		face.point3 = vertices[it.v[2] - 1];
		_faces.push_back(face);
	}
}

void MeshModel::set_renderer(Renderer * renderer)
{
	_renderer = renderer;
}

vec2 MeshModel::transform_point(const vec4 point) const
{
	const vec4 translated_point = _model_to_world * point;
	return vec2(translated_point.x / translated_point.w, translated_point.y / translated_point.w);
}

void MeshModel::draw()
{
	for(auto face : _faces)
	{
		auto point1 = transform_point(face.point1);
		auto point2 = transform_point(face.point2);
		auto point3 = transform_point(face.point3);

		//_renderer->draw_point(point1);
		//_renderer->draw_point(point2);
		//_renderer->draw_point(point3);
		
		_renderer->draw_line(point1, point2);
		_renderer->draw_line(point2, point3);
		_renderer->draw_line(point3, point1);
	}
}