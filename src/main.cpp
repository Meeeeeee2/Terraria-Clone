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
       
        if (IsKeyDown(KEY_SPACE)) {
            player.Jump();
        }
        else {
            if (!player.Grounded()) {
                player.canJump = false;
            }
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


        
        if (IsKeyDown(KEY_ONE)) {
            player.selectedBlock = DIRT;
        }
        if (IsKeyDown(KEY_TWO)) {
            player.selectedBlock = GRASS;
        }
        if (IsKeyDown(KEY_THREE)) {
            player.selectedBlock = STONE;
        }
        if (IsKeyDown(KEY_FOUR)) {
            player.selectedBlock = AIR;
        }
       

        //--------------
        // Mouse Input 
        // -------------
        Vector2 mousePos = GetMousePosition();
        mousePos = Vector2Add(mousePos, camera.position);
        mousePos.x = floor(mousePos.x / TILE_SIZE);
        mousePos.y = floor(mousePos.y / TILE_SIZE);

        if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT)) {
            SetTile(mousePos.x, mousePos.y, player.selectedBlock);
        }






        player.Update();
        camera.Center(player.position);



        BeginDrawing();
        ClearBackground({ 92, 148, 252,255 });

        // Draws background 
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