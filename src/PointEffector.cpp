#include "PointEffector.h"

void PointEffector::Apply(std::vector<Body>& bodies)
{
	for (auto& body : bodies)
	{
		Vector2 direction = body.position - position;
		if (Vector2Length(direction) <= size)
		{
			Vector2 force = Vector2Normalize(direction) * forceMagnitude;
			body.AddForce(force);
		}
	}
}

void PointEffector::Draw()
{
	DrawCircleLinesV(position, size, WHITE);
}