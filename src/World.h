#pragma once
#include "Body.h"
#include <vector>
#include "collision.h"

struct World 
{
public:
	World()
	{
		bodies.reserve(1000);
	}

	std::vector<Body> bodies;
	static Vector2 gravity;
	std::vector<class Effector*> effectors;
	std::vector<Contact> contacts;

	void Step(float dt);

	void Draw();

	void AddBody(const Body& body);
	void AddEffector(class Effector* effector); 

	void UpdateCollision();

	void Pull();

	void Repel();
};