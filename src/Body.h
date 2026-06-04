#pragma once

#include "raylib.h"
#include "raymath.h"

enum class BodyType
{
	Dynamic,
	Kinematic,
	Static
};

enum class ForceMode
{
	Force,
	Impulse,
	Acceleration,
	VelocityChange
};

struct Body
{
	BodyType bodyType = BodyType::Dynamic;
	Vector2 position{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	Vector2 acceleration{ 0, 0 };
	float size = 1.0f;
	float mass = 1.0f;
	float inverseMass = 1.0f; // 1/mass
	float restitution = 1.0f;
	float gravityScale = 1.0f;
	float damping = 0.0f;
	Color color = RED;

	void Step(float dt);
	void Draw() const;
	void AddForce(Vector2 force, ForceMode forceMode = ForceMode::Force);
};
