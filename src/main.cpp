#include <raylib.h>
using namespace std;

int main() {
    InitWindow(screenWidth, screenHeight, "Data Structure Visualization");
    SetTargetFPS(60);

    Menu menu;
    menu.init();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE); 
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
// g++ -Iinclude -ID:/raylib/include -o DSVisualization src/main.cpp include/var.h src/var.cpp include/ui.h src/ui.cpp -LD:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm