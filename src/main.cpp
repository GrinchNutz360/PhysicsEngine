/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "Body.h"
#include "World.h"
#include "Random.h"
#include "string"
#include "PointEffector.h"
#include "GravityEffector.h"
#include "AreaEffector.h"
#include "DragEffector.h"
#include "Spring.h"
#include "world.h"
#include "world_camera.h"

#include "raygui.h"
#include "raymath.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning ( push )
#pragma warning ( disable : 4576)
#include "gui_physics.h"
#pragma warning ( pop )

GuiPhysicsState state;

void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);


int main ()
{
	
	World world;
	SetRandomSeed(5);
	
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 10);
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));

	World::SetSpringMultiplier(1.0f);

	state = InitGuiPhysics();
	GuiLoadStyle("raygui/styles/dark/style_dark.rgs");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	//SetTargetFPS(10);

	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;

	float timeAccum = 0.0f;
	bool simulate = true;
	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = fminf(GetFrameTime(), 0.1f);
		float fixedTimeStep = 1.0f / state.FPSValue; // 0.016 * 60.0 = 1.0

		if (IsKeyPressed(KEY_SPACE)) simulate = !simulate;
		if (IsKeyPressed(KEY_TAB)) state.PhysicsPanelActive = !state.PhysicsPanelActive;

		world.gravity = Vector2{ 0, state.GravityValue };
		

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			selectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
		}
		// spring
		if (selectedBody)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			{
				Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
				if (IsKeyDown(KEY_LEFT_SHIFT))
				{
					Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 3.0f);
					selectedBody->AddForce(force);
				}
				else
				{
					connectedBody = world.GetBodyIntersect(world_camera.ScreenToWorld(GetMousePosition()));
				}
				DrawLineV(world_camera.WorldToScreen(position), world_camera.WorldToScreen(selectedBody->position), WHITE);
			}
			else
			{
				if (selectedBody && connectedBody)
				{
					float distance = (state.SpringAutoLengthChecked) ? Vector2Distance(selectedBody->position, connectedBody->position) : state.SpringLengthValue;
					world.AddSpring(*selectedBody, *connectedBody, distance, state.SpringStiffnessValue, state.SpringDampingValue);
				}

				selectedBody = nullptr;
				connectedBody = nullptr;
			}
		}
		Vector2 GuiAnchor = world_camera.ScreenToWorld(state.anchor02);
		Vector2 GuiValues = world_camera.ScreenToWorld(Vector2{304.0f, 664.0f});
		bool mouseOverGui = state.PhysicsPanelActive && CheckCollisionPointRec(world_camera.ScreenToWorld(GetMousePosition()), Rectangle{ GuiAnchor.x, GuiAnchor.y, GuiValues.x, GuiValues.y });
		if (!mouseOverGui) 
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				AddBody(world, world_camera);
			}
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_RIGHT_SHIFT))
			{
				AddEffector(world, world_camera);
			}
		}		
		//UPDATE
		if (state.SimulateActive)
		{
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}
		

		// DRAW
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		std::string fpsText = "FPS: ";
		fpsText += std::to_string(GetFPS());
		DrawText(fpsText.c_str(), GetScreenWidth() - 120, 40, 20, WHITE);

		//DrawCircleLinesV(GetMousePosition, state.BodySizeValue, )
		if (selectedBody)
		{
			DrawCircleLinesV(selectedBody->position, selectedBody->size * 1.05f, RED);
		}
		if (connectedBody)
		{
			DrawCircleLinesV(connectedBody->position, connectedBody->size * 1.05f, GREEN);
		}


		// draw our texture to the screen
		//DrawTexture(wabbit, 400, 200, WHITE);
		world_camera.Begin();
		world.Draw();
		world_camera.End();

		GuiPhysics(&state);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}

void AddBody(World& world, WorldCamera& camera)
{
	Body body;

	//body.bodyType = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Static : BodyType::Dynamic;
	body.bodyType = (BodyType)state.BodyTypeActive;

	body.position = camera.ScreenToWorld(GetMousePosition());
	float angle = GetRandomFloat() * (2 * PI);
	Vector2 direction;
	direction.x = cosf(angle);
	direction.y = sinf(angle);

	body.AddForce((direction * state.BodyVelocityValue), ForceMode::VelocityChange);

	body.acceleration = Vector2{ 0,0 };
	body.size = state.BodySizeValue * 0.5f;
	body.restitution = state.BodyRestitutionValue;
	body.mass = body.size * state.BodyMassValue;
	body.inverseMass = (body.bodyType == BodyType::Static) ? 0 : 1.0f / body.mass;
	body.gravityScale = state.BodyGravityValue;
	body.damping = state.BodyDampingValue;
	body.color = ColorFromHSV(GetRandomFloat(360.0f), 1.0f, 1.0f);

	world.AddBody(body);
}

void AddEffector(World& world, WorldCamera& camera)
{
	//world.AddEffector(new PointEffector(Vector2{ 200, 200 }, 100, 30000.0f));
	

	Vector2 position = camera.ScreenToWorld(GetMousePosition());

	Effector* effector = nullptr;
	switch ((EffectorType)state.EffectorTypeActive)
	{
	case EffectorType::Point:
			world.AddEffector(new PointEffector(position, state.EffectorSizeValue * 0.5f, state.EffectorForceValue));
		break;
	case EffectorType::Gravitation:
		world.AddEffector(new GravityEffector(position, state.EffectorSizeValue * 0.5f, state.EffectorForceValue));
		break;
	case EffectorType::Area:
		world.AddEffector(new AreaEffector(position, state.EffectorSizeValue * 0.5f, state.EffectorAngleValue, state.EffectorForceValue));
		break;
	case EffectorType::Drag:
		world.AddEffector(new DragEffector(position, state.EffectorSizeValue * 0.5f, state.EffectorForceValue));
		break;		
	}
	if (effector) world.AddEffector(effector);
}
