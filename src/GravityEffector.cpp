#include "GravityEffector.h"

void GravityEffector::Apply(std::vector<Body>& bodies)
{
	for (size_t i = 0; i < bodies.size(); i++)
	{
		for (size_t j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			// STEP 1: Direction vector
			Vector2 direction = bodyA.position - bodyB.position;
			//<direction vector points from bodyB to bodyA>

			// STEP 2: Distance
			float distance = Vector2Length(direction);
				//length of direction vector>

				// STEP 3: Clamp distance
			distance = fmaxf(distance, 1.0f);
					//<clamp distance to not go less than 1.0, use fmaxf()>

				// STEP 4: Force magnitude
			float forceMagnitude = strength * ((bodyA.mass * bodyB.mass) / (distance * distance));
					//<use formula in image above to compute force>;

			// STEP 5: Force vector
			Vector2 force = Vector2Normalize(direction) * forceMagnitude;
				//<scale normalized direction by force magnitude>

				// STEP 6: Apply equal and opposite forces
			bodyA.AddForce(force * -1);
			bodyB.AddForce(force);
		}
	}
}