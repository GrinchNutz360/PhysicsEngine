#pragma once
#include "Effector.h"

class PointEffector : public Effector
{
public:
	PointEffector(Vector2 position, float size, float forceMagnitude) :
		Effector{ position, size },
		forceMagnitude{ forceMagnitude }
	{}
	void Apply(std::vector<Body>& bodies) override;
	void Draw();

private:
	float forceMagnitude;
};