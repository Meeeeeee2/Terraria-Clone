#pragma once 
#include <raylib.h>
# include "Map.hpp"
constexpr float GRAVITY = 1600;

struct pCamera {
	Vector2 position;

	void Center(Vector2 center);
};
struct Player {
	
	const float moveSpeed = 200;
	const float jumpPower = 375;
	const float maxFallSpeed = 1000;
	
	BlockID selectedBlock = DIRT;

	Texture spriteSheet;

	Vector2 position;
	float verticalVelocity = 0;
	bool stationary = true;
	int moveDirection = 0;
	bool canJump = true;

	void Update();
	void Draw(Vector2 camPos);
	void Move(Vector2 velocity);
	void Jump();
	bool Grounded();
	Rectangle GetPlayerRect();


	Player();

private:

	const float animationDuration = 0.1f;
	const float maxJumpDuration = 0.2f;
	
	float jumpTimer = 0.0f;
	
	
	const float width = 31.5f;
	const int height = 44;

	float animationTimer = 0;
	bool grounded = true;
	

	bool CheckCollisions();
	
};