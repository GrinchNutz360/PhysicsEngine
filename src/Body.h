#pragma once

#include "raylib.h"
#include "raymath.h"


struct Body
{
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	float size;
	float mass;
	float restitution;

	void Step(float dt);
	void Draw() const;
	void AddForce(Vector2 force);
};
