#include <raylib.h>
#include "../include/var.h"
#include "../include/ui.h"
#include "../include/manager.h"
using namespace std;

int main() {
    InitWindow(screenWidth, screenHeight, "Data Structure Visualization");
    SetTargetFPS(60);

    init(); init_bg();
    current_state = MENU;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE); 
        event();
        draw();

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}