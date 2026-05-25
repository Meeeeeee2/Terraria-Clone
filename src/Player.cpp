#include "Player.hpp"
#include "Map.hpp"
#include <string>
#include <iostream>

void pCamera::Center(Vector2 center) {
	float newX = center.x - (0.5 * GetScreenWidth());
	float newY = center.y - (0.5 * GetScreenHeight());

	if (newX < 0) 
		newX = 0;

	if (newY < 0) 
		newY = 0;
	
	if (newX > (MAP_WIDTH * CHUNK_SIZE * TILE_SIZE) - GetScreenWidth())
	{
		newX = (MAP_WIDTH * CHUNK_SIZE * TILE_SIZE) - GetScreenWidth();
	}

	if (newY > (MAP_HEIGHT * CHUNK_SIZE * TILE_SIZE) - GetScreenHeight()) 
	{
		newY = (MAP_HEIGHT * CHUNK_SIZE * TILE_SIZE) - GetScreenWidth();
	}

	this->position.x = newX;
	this->position.y = newY;
}

void Player::Draw(Vector2 camPos) {
	int x = this->position.x - camPos.x;
	int y = this->position.y - camPos.y;
	
	int idx;

	if (!this->Grounded()) {
		idx = 1;
	}
	else if (this->stationary) {
		idx = 0;
	}
	
	else {
		idx = (int)(animationTimer / animationDuration) + 2;
	}
	Rectangle frameRight = { 0,0,-32,46 };
	frameRight.x = 4;
	frameRight.y = 7 + 56 * idx;

	Rectangle frameLeft = { 0,0,32,46 };
	frameLeft.x = 4;
	frameLeft.y = 7 + 56 * idx;

	DrawTextureRec(this->spriteSheet, 
		(this->moveDirection == 1 ? frameRight : frameLeft),
		{(float)x,(float)y},
		RAYWHITE);

}

void Player::Move(Vector2 translation) {
	this->position.x += translation.x;
	this->position.y += translation.y;

	if (translation.x > 0) {
		this->moveDirection = 1;
		this->stationary = false;
	}
	else if (translation.x < 0) {
		moveDirection = -1;
		this->stationary = false;
	}

	if (this->position.x < 0)
		this->position.x = 0;
	if (this->position.y < 0)
		this->position.y = 0;
}

bool Player::Grounded() {
	return true;
}

void Player::Update() {
	float dt = GetFrameTime();
	// Apply gravity
	this->verticalVelocity += GRAVITY * dt;
	// Clamp fall speed 
	if (this->verticalVelocity > this->maxFallSpeed) {
		this->verticalVelocity = this->maxFallSpeed;
	}

	this->Move({ 0,this->verticalVelocity * dt });

	if (this->moveDirection != 0) {
		this->animationTimer += dt;
		if (this->animationTimer > 14 * this->animationDuration) {
			this->animationTimer = 0;
		}
	}
	else {
		this->animationTimer = 0;
	}
}

void Player::Jump() {
	if (this->Grounded()) {
		this->verticalVelocity = -1 * this->jumpPower;
	}
}

Player::Player() {
	std::cout << "Start Load Player\n";
	std::string path = std::string(RESOURCES_PATH) + "Guide_Default.png";
	Image img = LoadImage(path.c_str());


	this->spriteSheet = LoadTextureFromImage(img);


	UnloadImage(img);
	std::cout << "LoadedPlayer\n";
}