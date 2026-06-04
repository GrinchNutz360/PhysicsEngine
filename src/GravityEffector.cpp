#include "GravityEffector.h"

void GravityEffector::Apply(std::vector<Body>& ibodies)
{
	std::vector<Body*> bodies;
	CollectBodiesInside(ibodies, bodies);
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = *bodies[i];
			Body& bodyB = *bodies[j];

			// calculate vector from bodyB to bodyA
			Vector2 direction = bodyA.position - bodyB.position;
			float distance = Vector2Length(direction);

			// prevent division by zero or extreme forces at very small distances
			distance = fmaxf(distance, 1.0f);

			// calculate gravitational force (F = G * ((m1 * m2) / d²))
			float forceMagnitude = strength * ((bodyA.mass * bodyB.mass) / (distance * distance));

			// get normalized direction and multiply by force magnitude
			Vector2 forceDirection = Vector2Normalize(direction) * forceMagnitude;

			// apply equal and opposite forces (Newton's third law)
			bodyA.AddForce(forceDirection * -1); // force bodyA towards bodyB
			bodyB.AddForce(forceDirection); // force bodyB towards bodyA
		}
	}
}
void GravityEffector::Draw()
{
	Effector::Draw();
	DrawCircleV(position, size, Fade(RED, 0.2f));
}