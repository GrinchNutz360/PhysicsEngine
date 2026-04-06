#include "Body.h"

void Body::Step(float dt)
{
	velocity += acceleration * dt;
	position += velocity * dt;
}

void Body::Draw() const
{
	DrawCircleV(position, size, RED);
}

void Body::AddForce(Vector2 force)
{
	acceleration += force / mass;
}


