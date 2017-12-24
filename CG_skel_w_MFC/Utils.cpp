#include "stdafx.h"
#include "Utils.h"

float clamp(float x, float lower, float upper)
{
	return min(upper, max(x, lower));
}

int min3(int x, int y, int z)
{
	return min(x, min(y, z));
}

int max3(int x, int y, int z)
{
	return max(x, max(y, z));
}
