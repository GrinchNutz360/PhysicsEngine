#pragma once
#include "raylib.h"
#include "raymath.h"


static inline float GetRandomFloat()
{
	return GetRandomValue(0, 10000) / (float)10000;
}

static inline float GetRandomFloat(float max)
{
	return GetRandomValue(0, max) / max;
}

static inline float GetRandomFloat(float min, float max)
{
	return GetRandomValue(min, max) / max;
}