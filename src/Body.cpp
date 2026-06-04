#include "Body.h"

void Body::Step(float dt)
{
	velocity += acceleration * dt;
	velocity *= (1.0f / (1.0f + damping * dt));
	position += velocity * dt;

}

void Body::Draw() const
{
	DrawCircleV(position, size, color);
	DrawCircleLinesV(position, size, WHITE);
}

void Body::AddForce(Vector2 force, ForceMode forceMode)
{
	if (bodyType != BodyType::Dynamic) return;

	switch (forceMode)
	{
	case ForceMode::Force:
		acceleration += force * inverseMass;
		break;
	case ForceMode::Impulse:
		velocity += force * inverseMass;
		break;
	case ForceMode::Acceleration:
		acceleration += force;
		break;
	case ForceMode::VelocityChange:
		velocity += force;
		break;
	default:
		break;
	}

}


