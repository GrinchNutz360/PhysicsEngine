#pragma once
#include "Body.h"
#include <vector>
struct World 
{
	std::vector<Body> bodies;
	Vector2 gravity;
	 
	void Step(float dt);

	void Draw();

	void AddBody(const Body& body);

	void Pull();

	void Repel();
};