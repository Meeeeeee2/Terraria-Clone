#pragma once
#include <cstdint>
#include <vector>
#include <raylib.h>
#include <FastNoiseLite.h>

constexpr int CHUNK_SIZE = 64;
constexpr int MAP_WIDTH = 200;
constexpr int MAP_HEIGHT= 60;
constexpr int TILE_SIZE = 16;

enum BlockID
{
	DIRT,
	STONE,
	GRASS,
	AIR
};

struct BlockDefinition
{
	uint16_t atlasX;
	uint16_t atlasY;
};
struct Tile 
{
	uint16_t blockID;

	Tile() {
		blockID = AIR;
	}

	Tile(uint16_t ID) {
		blockID = ID;
	}
};


struct Chunk 
{
	Tile tiles[CHUNK_SIZE * CHUNK_SIZE];

	Chunk() {
		tiles[0] = Tile(DIRT);
		tiles[63] = Tile(STONE);
		tiles[63 * 64] = Tile(GRASS);
		tiles[64 * 64 - 1] = Tile(DIRT);
		//tiles[1] = Tile(DIRT);
		//tiles[3] = Tile(DIRT);
		//tiles[63] = Tile(STONE);
		//tiles[100] = Tile(GRASS);
	}
};

extern BlockDefinition gBlocks[256];

extern std::vector<Chunk> Map;

extern Texture Tiles[3];


void CreateMap();

void RenderChunk(int x, int y);

void LoadTextures();

void RenderMap(int camLeft, int camTop);