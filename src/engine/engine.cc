
#include "engine.hh"
#include <iostream>
#include "raylib.h"
#include "raymath.h"
#include "screensize.hh"

namespace Engine {
    ResourceSprites::~ResourceSprites() {
        UnloadTexture(bricks);
        UnloadTexture(wood);
        UnloadTexture(wheat);
        UnloadTexture(cactus);
        UnloadTexture(random);
        UnloadTexture(stone);
        UnloadTexture(sheep);
    }

    float sqrt3 = static_cast<float>(sqrt(3));

    const Texture2D &get_texture_for_resource(const RenderResources &render_resources, const Map::Resource &resource) {
        switch (resource) {
            case Map::Resource::BRICK:
                return render_resources.resource_sprites.bricks;
            case Map::Resource::NONE:
                return render_resources.resource_sprites.cactus;
            case Map::Resource::SHEEP:
                return render_resources.resource_sprites.sheep;
            case Map::Resource::WHEAT:
                return render_resources.resource_sprites.wheat;
            case Map::Resource::STONE:
                return render_resources.resource_sprites.stone;
            case Map::Resource::WOOD:
                return render_resources.resource_sprites.wood;
        }
        return render_resources.resource_sprites.cactus;
    }

    Color get_color_for_resource(const ColorScheme &color_scheme, const Map::Resource &resource) {
        switch (resource) {
            case Map::Resource::BRICK:
                return color_scheme.brickRed;
            case Map::Resource::NONE:
                return color_scheme.desertSand;
            case Map::Resource::SHEEP:
                return color_scheme.grassGreen;
            case Map::Resource::WHEAT:
                return color_scheme.wheatYellow;
            case Map::Resource::STONE:
                return color_scheme.stoneGray;
            case Map::Resource::WOOD:
                return color_scheme.warmWoodBrown;
        }
        return BLACK;
    }


    Vector2 compute_hex_center_position(const RenderSettings &render_settings, const Map::HexCoord2 &hex_coord) {
        const float hex_size = render_settings.full_hex_size;
        return {.x = hex_size * 3.0f / 2.0f * static_cast<float>(hex_coord.q),
                .y = hex_size *
                     (sqrt3 / 2.0f * static_cast<float>(hex_coord.q) + sqrt3 * static_cast<float>(hex_coord.r))};
    }

    void render_map(const RenderSettings &render_settings, const RenderResources &render_resources,
                    const ColorScheme &color_scheme, const Map::Map &map) {
        std::string number;
        constexpr Vector2 divide_by_two{.x = 2.0, .y = 2.0};
        for (const auto [coord, hex]: map.get_hexes()) {
            const auto hex_position = compute_hex_center_position(render_settings, coord);
            DrawPoly(hex_position, 6, render_settings.hex_size, 0, get_color_for_resource(color_scheme, hex->resource));

            auto texture = get_texture_for_resource(render_resources, hex->resource);
            float scale = render_settings.hex_size * (2.5f / 3.0f) / static_cast<float>(texture.width);

            const Vector2 sprite_position = {
                    .x = hex_position.x - render_settings.hex_size * 1.0f / 3.0f -
                         scale * static_cast<float>(texture.width) / 2.0f,
                    .y = hex_position.y - render_settings.hex_size * 1.0f / 3.0f -
                         scale * static_cast<float>(texture.height) / 2.0f,
            };

            DrawTextureEx(get_texture_for_resource(render_resources, hex->resource), sprite_position, 0.0f, scale,
                          WHITE);

            const Vector2 circle_position = {
                    .x = hex_position.x + render_settings.hex_size * 1.0f / 3.0f,
                    .y = hex_position.y + render_settings.hex_size * 1.0f / 3.0f,
            };

            DrawCircleV(circle_position, render_settings.hex_size * 1.0f / 3.0f, color_scheme.mapBorder);

            number = std::to_string(hex->number);
            const auto font_size = MeasureTextEx(render_resources.map_font, number.c_str(), 32.0, 0.0f);
            const Vector2 final_text_position =
                    Vector2Subtract(circle_position, Vector2Divide(font_size, divide_by_two));
            DrawTextEx(render_resources.map_font, number.c_str(), final_text_position,
                       static_cast<float>(render_resources.map_font.baseSize), 0, WHITE);
            // DrawText(number.c_str(), final_text_position.x, final_text_position.y, 32, WHITE);
        }
    }

    void main_loop() {
        int start_screen_width = 0;
        int start_screen_height = 0;
#ifdef __APPLE__
        auto [width, height] = MacOS::get_screen_size();
        start_screen_width = static_cast<int>(width);
        // 28 to account for title bar
        start_screen_height = static_cast<int>(height - 28);
#endif // __APPLE__

        constexpr ColorScheme color_scheme = {.grassGreen = {83, 130, 75, 255},
                                              .warmWoodBrown = {136, 84, 24, 255},
                                              .stoneGray = {121, 121, 121, 255},
                                              .brickRed = {174, 32, 36, 255},
                                              .wheatYellow = {244, 201, 95, 255},
                                              .desertSand = {225, 169, 95, 255},
                                              .mapBorder = {51, 51, 51, 255},
                                              .color1 = {0, 112, 110, 255},
                                              .color2 = {255, 99, 71, 255},
                                              .color3 = {98, 44, 168, 255},
                                              .color4 = {218, 165, 32, 255}};

        InitWindow(start_screen_width, start_screen_height, "raylib [core] example - basic window");
        constexpr RenderSettings render_settings{.hex_size = 60, .full_hex_size = 70};

        const RenderResources render_resources = {
                .map_font = LoadFontEx("resources/OpenSans-Regular.ttf", 32, nullptr, 250),
                .resource_sprites = {.bricks = LoadTexture("resources/sprites/resources/resource_bricks.png"),
                                     .sheep = LoadTexture("resources/sprites/resources/resource_sheep.png"),
                                     .wood = LoadTexture("resources/sprites/resources/resource_wood.png"),
                                     .cactus = LoadTexture("resources/sprites/resources/resource_cactus.png"),
                                     .stone = LoadTexture("resources/sprites/resources/resource_stone.png"),
                                     .wheat = LoadTexture("resources/sprites/resources/resource_wheat.png"),
                                     .random = LoadTexture("resources/sprites/resources/resource_random.png")}};
        std::cout << ") Screen Width: " << GetScreenWidth() << "\n";
        std::cout << ") Screen Height: " << GetScreenHeight() << "\n";

        const auto screen_width = static_cast<float>(GetScreenWidth());
        const auto screen_height = static_cast<float>(GetScreenHeight());

        Camera2D camera{
                .offset = {0.0f, 0.0f},
                .target = {-screen_width / 2.0f, -screen_height / 2.0f},
                .rotation = 0.0f,
                .zoom = 1.0f,
        };

        Map::Map map = Map::Map::build_map_of_size(2);

        while (!WindowShouldClose()) {
            // render phase
            BeginDrawing();
            ClearBackground(color_scheme.mapBorder);
            BeginMode2D(camera);
            render_map(render_settings, render_resources, color_scheme, map);
            EndMode2D();

            DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), RED);
            DrawLine(0.0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED);
            DrawLineEx({0.0, screen_height - 5}, {screen_width, screen_height - 5}, 10, RED);

            EndDrawing();
        }
        UnloadFont(render_resources.map_font);
        CloseWindow();
    } // namespace Engine
} // namespace Engine
