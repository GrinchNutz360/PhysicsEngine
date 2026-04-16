#pragma once
#include "Body.h"
#include <vector>
struct Contact
{
	Body* bodyA;
	Body* bodyB;

	float restitution;	// restitution of both bodies
	float depth;		// penetration depth
	Vector2 normal;		// normal contact vector

};
void CreateContacts(std::vector<Body>& bodies, std::vector<Contact>& contacts);
void SeparateContacts(std::vector<Contact>& contacts);
void ResolveContacts(std::vector<Contact>& contacts);