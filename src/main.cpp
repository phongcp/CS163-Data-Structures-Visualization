#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/manager.h"
using namespace std;

int main() {
    // srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "Data Structure Visualization");
    InitAudioDevice();
    SetTargetFPS(60);

    init();
    current_state = MENU;

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        if(current_state == AVLTREE || current_state == TRIE || current_state == HASHTABLE){
            ClearBackground(isLightMode ? RAYWHITE : Color({31, 31, 31, 255})); // Clear the background
            DrawRectangle(0,0,screenWidth,60, isLightMode ? GRAY  : DARKGRAY); // Vẽ background
            DrawRectangle(0,screenHeight - 80,screenWidth,80, isLightMode ? GRAY : DARKGRAY); // Vẽ background
        }
        else ClearBackground(RAYWHITE);
        event();
        draw();

        EndDrawing();
    }

    // UnloadFont(customFont);
    // UnloadFont(fontPseudocode);
    // UnloadTexture(Logo);
    
    CloseWindow();
    return 0;
}