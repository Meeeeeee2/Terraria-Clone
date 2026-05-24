#include <iostream>
#include <raylib.h>
#include "Map.hpp"




int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 1000, "Terraria");

    SetExitKey(KEY_NULL);

    LoadTextures();
    CreateMap();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground({ 50,50,50,255 });



        RenderMap(0,0);
        
        EndDrawing();
    }
	return 0;
}