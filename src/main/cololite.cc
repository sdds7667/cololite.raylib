
#include "raylib.h"
#include "map.hh"

int main(void) {
  InitWindow(800, 450, "raylib [core] example - basic window");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", add(180, 10), 200, 20,
             LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
