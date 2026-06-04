#pragma once
#include "Body.h"
#include <vector>
#include "collision.h"
#include"Spring.h"

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


	void AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness, float damping);
	static void SetSpringMultiplier(float multiplier) { springMultiplier = multiplier; }
	Body* GetBodyIntersect(Vector2& position);

	void SetBounds(Vector2 min, Vector2 max) { boundsMin = min; boundsMax = max; }

	void UpdateCollision();

	void Pull();

	void Repel();

private:

	static float springMultiplier;

	std::vector<class Spring*> springs;
	Vector2 boundsMin{ -10.0f, -5.0f };
	Vector2 boundsMax{ 10.0f,  5.0f };
};