
#include "engine.hh"
#include <iostream>

#include "actor.hh"
#include "map_actors.hh"
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
    };


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

    float get_direction_for_corner(const Map::HexCornerDirection &corner_direction) {
        constexpr float sixty_degrees = PI / 3.0f;
        return sixty_degrees * static_cast<float>(corner_direction);
    }

    float get_direction_for_edge(const Map::HexEdgeDirection &edge_direction) {
        constexpr float sixty_degrees = PI / 3.0f;
        return static_cast<float>(edge_direction) * sixty_degrees;
    }


    Vector2 compute_hex_center_position(float hex_size, const Map::HexCoord2 &hex_coord) {
        return {
            .x = hex_size * 3.0f / 2.0f * static_cast<float>(hex_coord.q),
            .y = hex_size *
                 (sqrt3 / 2.0f * static_cast<float>(hex_coord.q) + sqrt3 * static_cast<float>(hex_coord.r))
        };
    }

    void render_map(const RenderSettings &render_settings, const RenderResources &render_resources,
                    const ColorScheme &color_scheme, const Map::Map &map) {
        std::string number;
        constexpr Vector2 divide_by_two{.x = 2.0, .y = 2.0};
        for (const auto [coord, hex]: map.get_hexes()) {
            const auto hex_position = compute_hex_center_position(render_settings.full_hex_size, coord);
            for (const auto coord2: Map::MapCoords(2)) {
                Color offset = get_color_for_resource(color_scheme, hex->resource);
                const int q = coord2.q;
                const int r = coord2.r;
                const int distance = (std::abs(coord2.q) + std::abs(coord2.q + coord2.r) + std::abs(coord2.r)) / 2;
                if (!((q == -2 && r <= 1) || (q == -1 && r <= 0) || (q == 0 && r <= 0))) {
                    int factor = 12;
                    offset.r -= distance * factor;
                    offset.g -= distance * factor;
                    offset.b -= distance * factor;
                }

                float little_size = render_settings.hex_size / 4.0f;
                const auto little_hex_position = compute_hex_center_position(
                    little_size, coord2);
                DrawPoly(Vector2Add(hex_position, little_hex_position), 6, little_size, 0,
                         offset);
            }

            auto texture = get_texture_for_resource(render_resources, hex->resource);
            float scale = render_settings.hex_size * (2.5f / 3.0f) / static_cast<float>(texture.width);

            const Vector2 sprite_center_position = {
                .x = hex_position.x - render_settings.hex_size * 1.0f / 3.0f,
                .y = hex_position.y - render_settings.hex_size * 1.0f / 3.0f
            };

            const Vector2 sprite_position = {
                .x = sprite_center_position.x - scale * static_cast<float>(texture.width) / 2.0f,
                .y = sprite_center_position.y - scale * static_cast<float>(texture.height) / 2.0f
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

        for (const auto [coord, edge]: map.get_edges()) {
            const auto hex_position = compute_hex_center_position(render_settings.full_hex_size, coord.hex_coord);
            const auto edge_direction = get_direction_for_edge(coord.edge_direction);
            const auto edge_position_scaling = Vector2{.x = cos(edge_direction), .y = sin(edge_direction)};
            const auto edge_delta = Vector2Scale(edge_position_scaling, render_settings.full_hex_size);
            const auto edge_position = Vector2Add(hex_position, edge_delta);

            float dir = static_cast<float>(coord.edge_direction);
            float move_dir = (PI / 3.0f) * (dir + 2.0);

            Rectangle rectangle{
                .x = edge_position.x + cos(move_dir) * render_settings.hex_size * 0.90f / 3.0f,
                .y = edge_position.y + sin(move_dir) * render_settings.hex_size * 0.90f / 3.0f,
                .width = render_settings.full_hex_size - 1.80f * render_settings.hex_size * 1.0f / 3.0f,
                .height = 19.0f
            };
            DrawRectanglePro(rectangle, {0.0f, 9.5f}, 60.0f * (static_cast<float>(coord.edge_direction) + 2.0f),
                             color_scheme.mapBorder);
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

        constexpr ColorScheme color_scheme = {
            .grassGreen = {83, 130, 75, 255},
            .warmWoodBrown = {136, 84, 24, 255},
            .stoneGray = {121, 121, 121, 255},
            .brickRed = {174, 32, 36, 255},
            .wheatYellow = {244, 201, 95, 255},
            .desertSand = {225, 169, 95, 255},
            // .mapBorder = {120, 120, 120, 255},
            // .circleBorder = {125, 125, 125, 255},
            .mapBorder = {30, 30, 30, 255},
            .circleBorder = {30, 30, 30, 255},
            .color1 = {0, 112, 110, 255},
            .color2 = {255, 99, 71, 255},
            .color3 = {98, 44, 168, 255},
            .color4 = {218, 165, 32, 255}
        };

        InitWindow(start_screen_width, start_screen_height, "raylib [core] example - basic window");
        constexpr RenderSettings render_settings{.hex_size = 60, .full_hex_size = 70};

        const RenderResources render_resources = {
            .map_font = LoadFontEx("resources/OpenSans-Regular.ttf", 32, nullptr, 250),
            .resource_sprites = {
                .bricks = LoadTexture("resources/sprites/resources/resource_bricks.png"),
                .sheep = LoadTexture("resources/sprites/resources/resource_sheep.png"),
                .wood = LoadTexture("resources/sprites/resources/resource_wood.png"),
                .cactus = LoadTexture("resources/sprites/resources/resource_cactus.png"),
                .stone = LoadTexture("resources/sprites/resources/resource_stone.png"),
                .wheat = LoadTexture("resources/sprites/resources/resource_wheat.png"),
                .random = LoadTexture("resources/sprites/resources/resource_random.png")
            },
            .sprites = {
                .house = LoadTexture("resources/sprites/house.png"),
            },

        };
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

        std::vector<Actor *> actors;
        AnimationSequence animation_sequence{};

        for (auto [coord, corner]: map.get_corners()) {
            const auto hex_position = compute_hex_center_position(render_settings.full_hex_size, coord.hex_coord);
            const auto corner_direction = get_direction_for_corner(coord.corner_direction);
            const auto corner_position_scaling = Vector2{.x = cos(corner_direction), .y = sin(corner_direction)};
            const auto corner_delta = Vector2Scale(corner_position_scaling, render_settings.full_hex_size);
            const auto corner_position = Vector2Add(hex_position, corner_delta);

            actors.push_back(new CornerActor(render_settings, color_scheme, corner, coord, corner_position));
        }

        while (!WindowShouldClose()) {
            if (IsKeyReleased(KEY_SPACE)) {
                for (auto actor: actors) {
                    auto *corner_actor = dynamic_cast<CornerActor *>(actor);
                    bool is_highlighted = corner_actor->get_highlighted();
                    corner_actor->set_highlighted(!is_highlighted);
                }
            }
            float delta_time = GetFrameTime();
            std::cout << "Rendering actors " << std::endl;
            for (auto actor: actors) {
                actor->update(delta_time);
            }
            std::cout << "Ticking animation sequence " << std::endl;
            animation_sequence.tick_animation(delta_time);
            // render phase
            BeginDrawing();
            ClearBackground({51, 51, 51, 255});
            BeginMode2D(camera);
            render_map(render_settings, render_resources, color_scheme, map);
            for (const auto &actor: actors) {
                actor->render();
            }
            EndMode2D();

            // DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), RED);
            // DrawLine(0.0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED);
            // DrawLineEx({0.0, screen_height - 5}, {screen_width, screen_height - 5}, 10, RED);
            DrawTextEx(render_resources.map_font, std::to_string(GetFPS()).c_str(), {30.0, 30.0}, 32.0, 2.0, WHITE);

            EndDrawing();
        }
        UnloadFont(render_resources.map_font);
        CloseWindow();
    } // namespace Engine
} // namespace Engine
