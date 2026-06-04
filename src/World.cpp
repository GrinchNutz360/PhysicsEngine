#include "World.h"
#include "Effector.h"

Vector2 World::gravity = { 0,  9.81f };
float World::springMultiplier = 1.0f;
void World::Step(float dt)
{
	for (auto& body : bodies) body.AddForce(gravity * body.gravityScale, ForceMode::Acceleration);
  
	for (auto& effector : effectors) effector->Apply(bodies);

	for (auto& spring : springs) spring->Apply(springMultiplier);

	//if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	//{
	//	Pull();
	//}
	//if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
	//{
	//	Repel();
	//}



	//force effector
	for (auto& effector : effectors) effector->Apply(bodies);
	for (auto& body : bodies) body.Step(dt);
	for (int i = 0; i < 4; i++) UpdateCollision();

	UpdateCollision();

	for (auto& body : bodies) body.acceleration = Vector2{ 0, 0 };
}

void World::Draw()
{
	DrawLineV(Vector2{ 0, boundsMin.y }, Vector2{ 0, boundsMax.y }, WHITE);
	for (float x = 1; x < (boundsMax.x - boundsMin.x); x += 1) 
	{
		DrawLineV(Vector2{ +x, boundsMin.y }, Vector2{ +x, boundsMax.y }, GRAY);
		DrawLineV(Vector2{ -x, boundsMin.y }, Vector2{ -x, boundsMax.y }, GRAY);
	}

	DrawLineV(Vector2{ boundsMin.x, 0 }, Vector2{ boundsMax.x, 0 }, WHITE);
	for (float y = 1; y < (boundsMax.y - boundsMin.y); y += 1)
	{
		DrawLineV(Vector2{ +y, boundsMin.x }, Vector2{ +y, boundsMax.x }, GRAY);
		DrawLineV(Vector2{ -y, boundsMin.x }, Vector2{ -y, boundsMax.x }, GRAY);
	}

	for (auto& effector : effectors) effector->Draw();
	for (auto& spring : springs) spring->Draw();
	for (const auto& body : bodies)
	{
		body.Draw();
	}
}

void World::AddBody(const Body & body)
{
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
}

void World::AddSpring(Body& bodyA, Body& bodyB, float restLength, float stiffness, float damping)
{
	Spring* spring = new Spring(&bodyA, &bodyB, restLength, stiffness);
	springs.push_back(spring);
}

Body* World::GetBodyIntersect(Vector2& position)
{
	for (auto& body : bodies)
	{
		if (CheckCollisionPointCircle(position, body.position, body.size))
		{
			return &body;
		}
	}
	return nullptr;
}

void World::Pull() 
{
	Vector2 position = GetMousePosition();
	for (auto& body : bodies)
	{
		Vector2 direction = position - body.position;
		if (Vector2Length(direction) <= 100.0f)
		{
			Vector2 force = Vector2Normalize(direction) * -20000.0f;
			body.AddForce(force);
		}
	}
	DrawCircleLinesV(position, 100, WHITE);
}

void World::Repel()
{
	Vector2 position = GetMousePosition();
	for (auto& body : bodies)
	{
		Vector2 direction = position - body.position;
		if (Vector2Length(direction) <= 100.0f)
		{
			Vector2 force = Vector2Normalize(direction) * 20000.0f;
			body.AddForce(force);
		}
	}
	DrawCircleLinesV(position, 100, RED);
}

void World::UpdateCollision()
{
	contacts.clear();
	CreateContacts(bodies, contacts);
	SeparateContacts(contacts);
	ResolveContacts(contacts);

	// collision
	for (auto& body : bodies)
	{
		if (body.position.x + body.size > boundsMax.x)
		{
			// body position x = bounds max x - body size
			body.position.x = (boundsMax.x - body.size);
			// set body velocity x with restitution
			body.velocity.x *= -body.restitution;

		}
		if (body.position.x - body.size < boundsMin.x)
		{
			// body position x = bounds min x + body size
			body.position.x = (boundsMin.x + body.size);
			// set body velocity x with restitution
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > boundsMax.y)
		{
			// body position y
			body.position.y = (boundsMax.y - body.size);
			// body velocity y
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < boundsMin.y)
		{
			// body position y
			body.position.y = (boundsMin.y + body.size);
			// body velocity y
			body.velocity.y *= -body.restitution;
		}
	}
}
//body.velocity.y *= - body.restitution

