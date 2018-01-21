#pragma once
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

struct FaceIdcs
{
	// TODO: why 4?
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i = 0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	explicit FaceIdcs(std::istream & aStream)
	{
		for (int i = 0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for (int i = 0; i < 3; i++)
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

inline vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

inline vec4 vec4fFromVec3Stream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec4(x, y, z, 1);
}

inline vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}
