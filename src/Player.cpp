#include "Player.hpp"
#include "Map.hpp"
#include <string>

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
	// !!!!!!! FINISH ADDING THE PROPER SPRITE !!!!!!!!!!
	Rectangle tile = { 0,0,30,46 };
	tile.x = 4;
	tile.y = 7 + 58;

	// ENGULFED IS IN PROGRESS !!!!!
	DrawRectangle(x, y, 48, 96, RED);
}

void Player::Move(Vector2 translation) {
	this->position.x += translation.x;
	this->position.y += translation.y;
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

}

void Player::Jump() {
	if (this->Grounded()) {
		this->verticalVelocity = -1 * this->jumpPower;
	}
}

Player::Player() {
	std::string path = std::string(RESOURCES_PATH) + "Guide_Default.png";
	Image img = LoadImage(path.c_str());

	this->spriteSheet = LoadTextureFromImage(img);

	UnloadImage(img);
}