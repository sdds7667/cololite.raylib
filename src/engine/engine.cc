
#include "engine.hh"
#include "raylib.h"
#include "screensize.hh"
#include <iostream>

namespace Engine {

float sqrt3 = sqrt(3);

const Vector2 compute_hex_center_position(const RenderSettings &render_settings,
                                          const Map::HexCoord2 &hex_coord) {
  float hex_size = render_settings.full_hex_size;
  return {.x = hex_size * 3.0f / 2.0f * static_cast<float>(hex_coord.q),
          .y = hex_size *
               ((sqrt3 / 2.0f * hex_coord.q) + (sqrt3 * hex_coord.r))};
}

void render_map(const RenderSettings &render_settings, const Map::Map &map) {
  for (const auto [coord, hex] : map.get_hexes()) {
    const auto hex_position =
        compute_hex_center_position(render_settings, coord);
    DrawPoly(hex_position, 6, render_settings.hex_size, 0, BLACK);
  }
}

void render(RenderTexture2D &texture, const RenderSettings &render_settings,
            const Map::Map &map) {

  BeginTextureMode(texture);
  render_map(render_settings, map);
  EndTextureMode();
};

void main_loop() {

  int start_screen_width = 0;
  int start_screen_height = 0;

#ifdef __APPLE__
  Vector2 screen_size = MacOS::get_screen_size();
  start_screen_width = screen_size.x;
  // 28 to account for title bar
  start_screen_height = screen_size.y - 28;

#endif // __APPLE__
  InitWindow(start_screen_width, start_screen_height,
             "raylib [core] example - basic window");
  RenderSettings render_settings{.hex_size = 60, .full_hex_size = 70};

  std::cout << ") Screen Width: " << GetScreenWidth() << "\n";
  std::cout << ") Screen Height: " << GetScreenHeight() << "\n";

  Camera2D camera{0.0};
  camera.offset = {0.0f, 0.0f};
  camera.target = {-GetScreenWidth() / 2.0f, -GetScreenHeight() / 2.0f};
  camera.zoom = 1.0f;
  camera.rotation = 0.0f;

  Map::Map map = Map::Map::build_map_of_size(2);

  while (!WindowShouldClose()) {

    // render phase
    BeginDrawing();
    ClearBackground(WHITE);
    BeginMode2D(camera);
    render_map(render_settings, map);
    EndMode2D();

    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();

    DrawLine(GetScreenWidth() / 2.0, 0, GetScreenWidth() / 2.0,
             GetScreenHeight(), RED);
    DrawLine(0.0, GetScreenHeight() / 2.0f, GetScreenWidth(),
             GetScreenHeight() / 2.0f, RED);
    DrawLineEx({0.0, screen_height - 5}, {screen_width, screen_height - 5}, 10,
               RED);

    EndDrawing();
  }

  CloseWindow();
}

} // namespace Engine
