#include "collision.h"
#include "Random.h"

bool Intersects(const Body& bodyA, const Body& bodyB)
{
	float distanceSqr = Vector2DistanceSqr(bodyA.position, bodyB.position);
	float radius = bodyA.size + bodyB.size;

	return (distanceSqr <= (radius * radius)); // check if distance square is less than equal to radius square
}

void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts)
{
	for (int i = 0; i < bodies.size(); i++)
	{
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Body& bodyA = bodies[i];
			Body& bodyB = bodies[j];

			if (bodyA.bodyType != BodyType::Dynamic && bodyB.bodyType != BodyType::Dynamic) continue;

			if (Intersects(bodyA, bodyB))
			{
				Contact contact;
				contact.bodyA = &bodyA;
				contact.bodyB = &bodyB;

				Vector2 direction = bodyA.position - bodyB.position;
				float distanceSqr = Vector2LengthSqr(direction); // avoids square root

				// c2 = a2 + b2
				// c = sqrt(a2 + b2)
				//float lengthSqr = (direction.x * direction.x) + (direction.y * direction.y);
				//float length = sqrtf(lengthSqr);
				if (distanceSqr <= EPSILON)
				{
					direction = Vector2{ GetRandomFloat(-0.05f, 0.05f), GetRandomFloat(-0.05f, 0.05f) };
					distanceSqr = Vector2LengthSqr(direction); // avoids square root
				}

				float distance = sqrtf(distanceSqr); // use square root to get actual distance
				float radius = bodyA.size + bodyB.size;
				contact.depth = radius - distance;
				contact.normal = Vector2Normalize(direction);
				contact.restitution = (bodyA.restitution + bodyB.restitution) * 0.5f;
				contacts.push_back(contact);
			}
		}
	}
}

void SeparateContacts(std::vector<Contact>& contacts)
{
	for (auto& contact : contacts)
	{
		float totalInverseMass = contact.bodyA->inverseMass + contact.bodyB->inverseMass;
		Vector2 separation = contact.normal * (contact.depth/totalInverseMass);
		contact.bodyA->position = contact.bodyA->position + (separation * contact.bodyA->inverseMass);
		contact.bodyB->position = contact.bodyB->position - (separation * contact.bodyB->inverseMass);
	}
}