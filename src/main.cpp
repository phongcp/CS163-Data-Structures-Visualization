#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/manager.h"
using namespace std;

int main() {
    srand(time(NULL));
    InitWindow(screenWidth, screenHeight, "Data Structure Visualization");
    SetTargetFPS(60);

    init();
    // current_state = MENU;
    current_state = AVLTREE;

    while (!WindowShouldClose()) {
        BeginDrawing();

        // ClearBackground(isLightMode ? RAYWHITE : ConvertToDarkMode(RAYWHITE)); // Clear the background
        // DrawRectangle(0,0,screenWidth,60, isLightMode ? DARKGRAY : ConvertToDarkMode(DARKGRAY)); // Vẽ background
        // DrawRectangle(0,screenHeight - 80,screenWidth,80, isLightMode ? DARKGRAY : ConvertToDarkMode(DARKGRAY)); // Vẽ background

        ClearBackground(RAYWHITE);
        event();
        draw();

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}