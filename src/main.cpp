#include <iostream>
#include <raylib.h>
#include "Map.hpp"
#include "Player.hpp"
#include <raymath.h>

pCamera camera;


int main()
{

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1000, 1000, "Terraria");

    Player player;

    SetExitKey(KEY_NULL);

    LoadTextures();
    DefineBlocks();
    CreateMap();

    while (!WindowShouldClose())
    {
        //  --------
        // Input Handling
        // ---------
        float dt = GetFrameTime();

        if (IsKeyDown(KEY_W)) {
            player.Move({ 0, player.moveSpeed * -1  * dt});
        }
        if (IsKeyDown(KEY_S)) {
            player.Move({ 0, player.moveSpeed * dt});
        }

        if (IsKeyPressed(KEY_SPACE)) {
            player.Jump();
        }
        if (IsKeyDown(KEY_A)) {
            player.Move({ player.moveSpeed * -1 * dt,0});
        }
        else if (IsKeyDown(KEY_D)) {
            player.Move({  player.moveSpeed  * dt,0 });
        }
        else {
            player.stationary = true;
        }

        player.Update();
        camera.Center(player.position);

        BeginDrawing();
        ClearBackground({ 92, 148, 252,255 });
        // background 
        DrawRectangleGradientV(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Color{ 92,148,252,255 },
            Color{ 180,220,255,255 }
        );

        
        RenderMap(camera.position);
        
        player.Draw(camera.position);

        EndDrawing();
    }
	return 0;
}