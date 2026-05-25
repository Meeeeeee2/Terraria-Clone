#pragma once 
#include <raylib.h>

constexpr float GRAVITY = 800;

struct pCamera {
	Vector2 position;

	void Center(Vector2 center);
};
struct Player {
	
	const float moveSpeed = 200;
	const float jumpPower = 450;
	const float maxFallSpeed = 1000;
	
	Texture spriteSheet;

	Vector2 position;
	float verticalVelocity = 0;
	bool stationary = true;
	int moveDirection = 0;
	

	void Update();
	void Draw(Vector2 camPos);
	void Move(Vector2 velocity);
	void Jump();
	bool Grounded();
	Rectangle GetPlayerRect();


	Player();

private:

	const float animationDuration = 0.1f;


	bool CheckCollisions();
	const int width = 32;
	const int height = 44;

	float animationTimer = 0;
	
	
};