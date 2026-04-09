#include "World.h"
#include "Effector.h"

Vector2 World::gravity = { 0,  9.81f };
void World::Step(float dt)
{
	for (auto& body : bodies) body.acceleration = Vector2{ 0,0 };
	for (auto& effector : effectors) effector->Apply(bodies);

	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		Pull();
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE))
	{
		Repel();
	}

	//force effector
	for (auto& effector : effectors) effector->Apply(bodies);
	for (auto& body : bodies) body.Step(dt);

	UpdateCollision();
	/*
	for (auto& body : bodies)
	{
		if (body.position.x + body.size > GetScreenWidth())
		{
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < 0)
		{
			body.position.x = body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > GetScreenHeight())
		{
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < 0)
		{
			body.position.y = body.size;
			body.velocity.y *= -body.restitution;
		}
	}
	*/
}

void World::Draw()
{
	for (const auto& body : bodies)
	{
		body.Draw();
	}
	for (auto& effector : effectors) effector->Draw();
}

void World::AddBody(const Body & body)
{
	bodies.push_back(body);
}

void World::AddEffector(Effector* effector)
{
	effectors.push_back(effector);
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

	// collision
	for (auto& body : bodies)
	{
		if (body.position.x + body.size > GetScreenWidth())
		{
			body.position.x = GetScreenWidth() - body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.x - body.size < 0)
		{
			body.position.x = body.size;
			body.velocity.x *= -body.restitution;
		}
		if (body.position.y + body.size > GetScreenHeight())
		{
			body.position.y = GetScreenHeight() - body.size;
			body.velocity.y *= -body.restitution;
		}
		if (body.position.y - body.size < 0)
		{
			body.position.y = body.size;
			body.velocity.y *= -body.restitution;
		}
	}
}


