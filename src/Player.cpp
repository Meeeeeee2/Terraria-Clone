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
	
	if (!this->CheckCollisions()) 
	{
		// sets flags for the sprite rendering 
		if (translation.x > 0) 
		{
			this->moveDirection = 1;
			this->stationary = false;
		}
		else if (translation.x < 0) 
		{
			moveDirection = -1;
			this->stationary = false;
		}
		// checks it is in bounds 
		if (this->position.x < 0)
			this->position.x = 0;
	}
	else {
			this->position.x -= translation.x;
	}

	//used for jumps and gravity 
	this->position.y += translation.y;

	if (!this->CheckCollisions()) 
	{
		if (this->position.y < 0)
			this->position.y = 0;

		this->grounded = false;
	}
	else 
	{
		this->position.y -= translation.y;
		this->verticalVelocity = 0;
		this->grounded = true;
	}
	
}

bool Player::Grounded() {
	return this->grounded;
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

	// player animation logic 
	if (this->moveDirection != 0) {
		this->animationTimer += dt;
		if (this->animationTimer > 14 * this->animationDuration) {
			this->animationTimer = 0;
		}
	}
	else {
		this->animationTimer = 0;
	}
	// reset jump when on floor
	if (this->Grounded()) {
		this->jumpTimer = 0.0f;
		this->canJump = true;
	}
}

void Player::Jump() {     
	bool setVelocity = false;

	
	if (this->Grounded()) {
		setVelocity = true;
	}
	else {
		if (canJump &&
			jumpTimer > 0 &&
			jumpTimer < this->maxJumpDuration) {
			setVelocity = true;
		}
	}
	if (setVelocity) {
		this->verticalVelocity = -1 * this->jumpPower;
		jumpTimer += GetFrameTime();
	}
	
}

Player::Player() {

	std::string path = std::string(RESOURCES_PATH) + "Guide_Default.png";
	Image img = LoadImage(path.c_str());


	this->spriteSheet = LoadTextureFromImage(img);


	UnloadImage(img);


	this->position = { 10,10 };
}

Rectangle Player::GetPlayerRect() {
	return {
		this->position.x,
		this->position.y,
		(float)this->width,
		(float)this->height
	};
}

bool Player::CheckCollisions() {
	// define bounds of where could be colliding 
	int top = floor(this->position.y / TILE_SIZE);
	int bottom = floor((this->position.y + this->height) / TILE_SIZE);
	int left = floor(this->position.x / TILE_SIZE);
	int right = floor((this->position.x + this->width) / TILE_SIZE);

	

	// check for collision
	Rectangle playerRec = this->GetPlayerRect();

	for (int y = top; y <= bottom; y++) {
		for (int x = left; x <= right; x++) {
			if (IsSolid(x, y)) {
				Rectangle tile = {(float) x * TILE_SIZE, (float)y * TILE_SIZE, (float)TILE_SIZE, (float)TILE_SIZE };
				if (CheckCollisionRecs(playerRec, tile)) {
					return true;
				}
			}
		}
	}
	return false;
	
}