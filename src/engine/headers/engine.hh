
#pragma once
#include "coords.hh"
#include "map.hh"
#include "raylib.h"

namespace Engine {
    struct ColorScheme {
        Color grassGreen;
        Color warmWoodBrown;
        Color stoneGray;
        Color brickRed;
        Color wheatYellow;
        Color desertSand;
        Color mapBorder;
        Color color1;
        Color color2;
        Color color3;
        Color color4;
    };

    struct ResourceSprites {
        Texture2D bricks;
        Texture2D sheep;
        Texture2D wood;
        Texture2D cactus;
        Texture2D stone;
        Texture2D wheat;
        Texture2D random;

        ~ResourceSprites();
    };

    struct RenderResources {
        Font map_font;
        ResourceSprites resource_sprites;
    };


    struct RenderSettings {
        float hex_size;
        float full_hex_size;
    };

    Vector2 compute_hex_center_position(const RenderSettings &render_settings, const Map::HexCoord2 &hex_coord);

    void render_map(const RenderSettings &render_settings, const RenderResources &render_resources,
                    const ColorScheme &color_scheme, const Map::Map &map);
    void main_loop();
} // namespace Engine
