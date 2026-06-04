#pragma once
#include "Effector.h"

class GravityEffector : public Effector
{
public:
    GravityEffector(Vector2 position, float size, float strength) : Effector(position, size), strength(strength) {}

    void Apply(std::vector<Body>& bodies) override;
    virtual void Draw();

private:
    float strength;
};