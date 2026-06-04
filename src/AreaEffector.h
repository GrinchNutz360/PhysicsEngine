#pragma once
#include "Effector.h"

class AreaEffector : public Effector
{
public:
	AreaEffector(Vector2 position, float size, float angle, float forceMagnitude) :
		Effector{ position, size },
		forceMagnitude{ forceMagnitude },
		angle{angle}
	{}
	void Apply(std::vector<Body>& bodies) override;
	void Draw();

private:
	float forceMagnitude;
	float angle;
};