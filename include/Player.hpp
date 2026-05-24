#pragma once 
#include <raylib.h>

constexpr float GRAVITY = 1500;

struct pCamera {
	Vector2 position;

	void Center(Vector2 center);
};
struct Player {
	
	float moveSpeed = 1000;
	float jumpPower = 800;
	float maxFallSpeed = 1000;
	Texture spriteSheet;

	Vector2 position;
	float verticalVelocity = 0;

	void Update();
	void Draw(Vector2 camPos);
	void Move(Vector2 velocity);
	void Jump();
	bool Grounded();

	Player();
};