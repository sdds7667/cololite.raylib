
#pragma once
#include "coords.hh"
#include "map.hh"
#include "raylib.h"

namespace Engine {

struct RenderSettings {
  float hex_size;
  float full_hex_size;
};

Vector2 compute_hex_center_position(const RenderSettings &render_settings,
                                          const Map::HexCoord2 &hex_coord);

void render_map(const RenderSettings &render_settings, const Map::Map &map);
void main_loop();
} // namespace Engine
