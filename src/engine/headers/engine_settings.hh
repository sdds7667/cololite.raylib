//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ENGINE_SETTINGS_HH
#define COLOLITE_ENGINE_SETTINGS_HH
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
        Color circleBorder;
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
        Texture2D resource_outline;
        Texture2D resource_stack;
        Texture2D trade;

        ~ResourceSprites();
    };

    struct Sprites {
        Texture2D house;
    };

    struct RenderResources {
        Font map_font;
        ResourceSprites resource_sprites;
        Sprites sprites;
    };

    struct RenderSettings {
        float hex_size;
        float full_hex_size;
    };
}

#endif //COLOLITE_ENGINE_SETTINGS_HH
