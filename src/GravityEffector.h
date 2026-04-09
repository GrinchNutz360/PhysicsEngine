#pragma once
#include "Effector.h"

class GravityEffector : public Effector
{
public:
    GravityEffector(float strength) : strength(strength) {}

    void Apply(std::vector<Body>& bodies) override;

private:
    float strength;
};