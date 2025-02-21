#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
using namespace std;

int main() {
    InitWindow(screenWidth, screenHeight, "Data Structure Visualization");
    SetTargetFPS(60);

    Menu menu;
    menu.init();
    // Interface ITF;
    // ITF.init();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE); 
        draw_bg();
        menu.draw();

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}