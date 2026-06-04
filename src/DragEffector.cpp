#include "DragEffector.h"

void DragEffector::Apply(std::vector<Body>& ibodies)
{
	//drag => -vk
	//force = Vector2Negate(body.velocity) * drag;
	//body.AddForce(force)
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);

	for (auto& body : bodies)
	{
		Vector2 force = Vector2Negate(body->velocity) * drag;
		body->AddForce(force);
	}
}

void DragEffector::Draw() 
{
	Effector::Draw();
	DrawCircleV(position, size, Fade(YELLOW, 0.2f));
}
