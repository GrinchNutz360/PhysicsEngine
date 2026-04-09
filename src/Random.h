#pragma once
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

static inline float GetRandomFloat()
{
	return GetRandomValue(0, 10000) / (float)10000;
}

static inline float GetRandomFloat(float max)
{
	return GetRandomFloat() * max;
}

static inline float GetRandomFloat(float min, float max)
{
	if (min > max) std::swap(min, max);
	return min + GetRandomFloat() * (max - min);
}