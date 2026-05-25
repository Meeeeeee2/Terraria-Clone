#include "Map.hpp"
#include <string>
#include <iostream>
#include <cpp/FastNoiseLite.h>


BlockDefinition gBlocks[256];

TileVariant variants[16];
bool variantsSetup = false;

std::vector<Chunk> Map;

Texture Tiles[3];

FastNoiseLite noise;

FastNoiseLite variationNoise;
bool noiseSetup = false;


// World Gen Settings
int averageSurfaceHeight = 120;
int surfaceVariance = 40;
int dirtThickness = 5;

template <typename T>
void clamp(T *num,T min, T max){
	if (*num < min) {
		*num = min;
	}
	if (*num > max) {
		*num = max;
	}
}


void SetupNoise() {
	//sets up the noise funcion so that it is easily usable 
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFrequency(0.003f);

	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);


	variationNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	variationNoise.SetFrequency(0.1f);

	variationNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
	variationNoise.SetFractalOctaves(5);

	noiseSetup = true;
}

void setupVariants() {
	
	variants[0].atlasX = 9;
	variants[0].atlasY = 3;

	variants[1].atlasX = 6;
	variants[1].atlasY = 3;

	variants[2].atlasX = 9;
	variants[2].atlasY = 0;

	variants[3].atlasX = 2;
	variants[3].atlasY = 4;

	variants[4].atlasX = 6;
	variants[4].atlasY = 0;

	variants[5].atlasX = 5;
	variants[5].atlasY = 0;

	variants[6].atlasX = 2;
	variants[6].atlasY = 3;

	variants[7].atlasX = 0;
	variants[7].atlasY = 0;

	variants[8].atlasX = 12;
	variants[8].atlasY = 0;

	variants[9].atlasX = 1;
	variants[9].atlasY = 4;

	variants[10].atlasX = 6;
	variants[10].atlasY = 4;

	variants[11].atlasX = 1;
	variants[11].atlasY = 2;

	variants[12].atlasX = 3;
	variants[12].atlasY = 3;

	variants[13].atlasX = 4;
	variants[13].atlasY = 0;

	variants[14].atlasX = 1;
	variants[14].atlasY = 0;

	variants[15].atlasX = 1;
	variants[15].atlasY = 1;


	variantsSetup = true;
}

void DefineBlocks() 
{
	//defines traits of each block type 
	gBlocks[DIRT].solid = true;
	gBlocks[STONE].solid = true;
	gBlocks[GRASS].solid = true;
	gBlocks[AIR].solid = false;
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
	
	for (int chunkY = 0; chunkY < MAP_HEIGHT; chunkY++)
	{
		for (int chunkX = 0; chunkX < MAP_WIDTH; chunkX++)
		{
			Chunk c;
			for (int localY = 0; localY < CHUNK_SIZE; localY++)
			{
				for (int localX = 0; localX < CHUNK_SIZE; localX++)
				{
					int worldX = chunkX * CHUNK_SIZE + localX;
					int worldY = chunkY * CHUNK_SIZE + localY;
					int idx = localY * CHUNK_SIZE + localX;

					if (worldY < averageSurfaceHeight - surfaceVariance) {
						c.tiles[idx].blockID = AIR;
					}
					else if (worldY > averageSurfaceHeight + surfaceVariance) {
						c.tiles[idx].blockID = STONE;
					}
					else {
						int surface = GetSurfaceHeight(worldX);

						if (worldY < surface) {
							c.tiles[idx].blockID = AIR;
						}
						else if (worldY == surface) {
							c.tiles[idx].blockID = GRASS;
						}
						else if (worldY < surface + dirtThickness) {
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

void RenderChunk(int ChunkX, int ChunkY, Vector2 camPos) {
	
	if (!noiseSetup) SetupNoise();
	if (!variantsSetup) setupVariants();
	// quit if not in bounds 
	if ((ChunkY * MAP_WIDTH + ChunkX) >= Map.size()){	
		return;
	}

	// get reference to current chunk 
	Chunk& chunk = Map[ChunkY * MAP_WIDTH + ChunkX];

	for (int y = 0; y < CHUNK_SIZE; y++) 
	{
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			Rectangle tile = { 0,0,16,16 };
			
			int worldX = ChunkX * CHUNK_SIZE + x;
			int worldY = ChunkY * CHUNK_SIZE + y;
			
			// checks which sides are solid 
			int mask = 0;

			if (IsSolid(worldX, worldY - 1)) mask |= 1;
			if (IsSolid(worldX + 1, worldY)) mask |= 2;
			if (IsSolid(worldX, worldY + 1)) mask |= 4;
			if (IsSolid(worldX - 1, worldY)) mask |= 8;

			float n = variationNoise.GetNoise((float)x, (float)y);
			n = (n + 1) * 0.5;
			n = n * 2.9999999f;
			int temp = (int)n;
			clamp(&temp, 0, 2);

			tile.x = variants[mask].atlasX * 18;
			tile.y = variants[mask].atlasY * 18;
				
			if (mask == 15) {
				tile.x = (variants[mask].atlasX + temp) * 18;
			}
			
			uint16_t tileMap = chunk.tiles[y * CHUNK_SIZE + x].blockID;
			if (tileMap == AIR) {
				continue;
			}
			DrawTextureRec(Tiles[tileMap], 
				tile, 
				Vector2{ (ChunkX * CHUNK_SIZE * TILE_SIZE + float(x) * TILE_SIZE) - camPos.x,
				(ChunkY * CHUNK_SIZE * TILE_SIZE + float(y) * TILE_SIZE) - camPos.y},
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

void RenderMap(Vector2 pos) {
	int startChunkX = pos.x / (CHUNK_SIZE * TILE_SIZE);
	int endChunkX = startChunkX + (GetScreenWidth() / (CHUNK_SIZE * TILE_SIZE)) + 1;

	int startChunkY = pos.y / (CHUNK_SIZE * TILE_SIZE);
	int endChunkY = startChunkY + (GetScreenHeight() / (CHUNK_SIZE * TILE_SIZE)) + 1;

	for (int y = startChunkY; y <= endChunkY; y++)
	{
		for (int x = startChunkX; x <= endChunkX ; x++)
		{

			RenderChunk(x, y, pos);
		}
	}
}

Tile& GetTile(int WTileX, int WTileY) {
	int chunkX = floor((float)WTileX / CHUNK_SIZE);
	int localX = WTileX % CHUNK_SIZE;

	int chunkY = floor((float)WTileY / CHUNK_SIZE);
	int localY = WTileY % CHUNK_SIZE;

	return Map[chunkY * MAP_WIDTH + chunkX]
		.tiles[localY * CHUNK_SIZE + localX];
}

bool IsSolid(int tileX, int tileY) {
	if (tileX >= 0 && tileX <= CHUNK_SIZE * MAP_WIDTH - 1 &&
		tileY >= 0 && tileY <= CHUNK_SIZE * MAP_HEIGHT - 1) {
		return gBlocks[
			GetTile(tileX, tileY).blockID]
			.solid;
	}
	else {
		return false;
	}
	
}

void SetTile(int WTileX, int WTileY, BlockID type) {
	GetTile(WTileX, WTileY).blockID = type;
}
