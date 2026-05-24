#include "Map.hpp"
#include <string>
#include <iostream>
#include <cpp/FastNoiseLite.h>


BlockDefinition gBlocks[256];

std::vector<Chunk> Map;

Texture Tiles[3];

FastNoiseLite noise;
bool noiseSetup = false;


// World Gen Settings
int averageSurfaceHeight = 50;
int surfaceVariance = 40;
int dirtThickness = 5;



void SetupNoise() {
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.003f);

	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);

	noiseSetup = true;
}

int GetSurfaceHeight(int WorldX) {
	float n = noise.GetNoise((float)WorldX, 0.0f);

	return averageSurfaceHeight + (int)(n * surfaceVariance);
}

void CreateMap()
{
	if (!noiseSetup) SetupNoise();
	
	Map.clear();
	Map.reserve(MAP_HEIGHT * MAP_WIDTH);
	
	for (int yC = 0; yC < MAP_HEIGHT; yC++)
	{
		for (int xC = 0; xC < MAP_WIDTH; xC++)
		{
			Chunk c;
			for (int yL = 0; yL < CHUNK_SIZE; yL++)
			{
				for (int xL = 0; xL < CHUNK_SIZE; xL++)
				{
					int xW = xC * CHUNK_SIZE + xL;
					int yW = yC * CHUNK_SIZE + yL;
					int idx = yL * CHUNK_SIZE + xL;

					if (yW < averageSurfaceHeight - surfaceVariance) {
						c.tiles[idx].blockID = AIR;
					}
					else if (yW > averageSurfaceHeight + surfaceVariance) {
						c.tiles[idx].blockID = STONE;
					}
					else {
						int surface = GetSurfaceHeight(xW);

						if (yW < surface) {
							c.tiles[idx].blockID = AIR;
						}
						else if (yW == surface) {
							c.tiles[idx].blockID = GRASS;
						}
						else if (yW < surface + dirtThickness) {
							c.tiles[idx].blockID = DIRT;
						}
						else {
							c.tiles[idx].blockID = STONE;
						}
					}
					

				}

			}
			Map.push_back(c);
		}
	}
	
}

void RenderChunk(int ChunkX, int ChunkY) {

	if ((ChunkY * MAP_WIDTH + ChunkX) >= Map.size()){
		
		return;
	}

	Chunk& chunk = Map[ChunkY * MAP_WIDTH + ChunkX];

	for (int y = 0; y < CHUNK_SIZE; y++) 
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			Rectangle tile = { 0,0,16,16 };
			tile.x = 1 * 18;
			tile.y = 1 * 18;
			uint16_t tileMap = chunk.tiles[y * CHUNK_SIZE + x].blockID;
			if (tileMap == AIR) {
				continue;
			}
			DrawTextureRec(Tiles[tileMap], 
				tile, 
				Vector2{ChunkX * CHUNK_SIZE * TILE_SIZE + float(x) * TILE_SIZE,
				ChunkY * CHUNK_SIZE * TILE_SIZE + float(y) * TILE_SIZE},
				WHITE);
		}
	}
}

void LoadTextures() {
	for (int i = 0; i < 3; i++) {
		std::string path = std::string(RESOURCES_PATH) + "/Tiles/tiles_" + std::to_string(i) + ".png";
		Image img = LoadImage(path.c_str());

		//ImageResizeNN(&img, TILE_SIZE, TILE_SIZE);

		Tiles[i] = LoadTextureFromImage(img);

		UnloadImage(img);
	}
	

}
void RenderMap(int camLeft, int camTop) {
	int startChunkX = camLeft / (CHUNK_SIZE * TILE_SIZE);
	int endChunkX = startChunkX + (GetScreenWidth() / (CHUNK_SIZE * TILE_SIZE)) + 1;

	int startChunkY = camTop / (CHUNK_SIZE * TILE_SIZE);
	int endChunkY = startChunkY + (GetScreenHeight() / (CHUNK_SIZE * TILE_SIZE)) + 1;

	for (int y = startChunkY; y <= endChunkY; y++)
	{
		for (int x = startChunkX; x <= endChunkX ; x++)
		{

			RenderChunk(x, y);
		}
	}
}
