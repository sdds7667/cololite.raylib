#include "engine.hh"
#include <iostream>
#include <random>
#include <ranges>
#include "actor.hh"
#include "game_sequence.hh"
#include "map.hh"
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

                float little_size = render_settings.hex_size / 3.5f;
                const auto little_hex_position = compute_hex_center_position(little_size, coord2);
                DrawPoly(Vector2Add(hex_position, little_hex_position), 6, little_size, 0, offset);
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
            DrawTextEx(render_resources.map_font, number.c_str(), final_text_position, 32.0f, 0, WHITE);
            // DrawText(number.c_str(), final_text_position.x, final_text_position.y, 32, WHITE);
        }
    }


    void render_rolls(const RenderResources &render_resources, const std::deque<Roll> &rolls) {
        constexpr float starting_x = 1920.0 / 2;
        constexpr float starting_y = 64.0f;
        constexpr float spacing = 10.0f;
        constexpr float scale = 1.0f;
        constexpr float circle_radius = 32.0f;
        for (size_t i = 0; i < 5; i++) {
            Vector2 circle_position = {
                .x = starting_x + (spacing + circle_radius * 2) * static_cast<float>(i),
                .y = starting_y,
            };
            std::string number = std::to_string(rolls.at(i).get_roll());
            Vector2 text_size = MeasureTextEx(render_resources.map_font, number.c_str(), 32.0, 0.0f);
            Vector2 text_position = Vector2Subtract(circle_position, Vector2Divide(text_size, {.x = 2.0f, .y = 2.0f}));
            DrawCircleV(circle_position, circle_radius, BLACK);
            DrawTextEx(render_resources.map_font, number.c_str(), text_position, 32.0, 0, WHITE);
        }
    }

    void enable_building_spots_with_roads(std::vector<Actor *> &actors) {
        for (auto actor: actors) {
            auto *corner_actor = dynamic_cast<CornerActor *>(actor);
            if (corner_actor == nullptr)
                continue;
            auto corner = corner_actor->get_corner();
            if (corner->house == nullptr) {
                bool can_build = false;
                for (auto edge: corner->edges | std::views::values) {
                    if (edge->road != nullptr) {
                        can_build = true;
                        break;
                    }
                }
                if (!can_build) {
                    continue;
                }
                for (auto edge: corner->edges | std::views::values) {
                    for (auto corner2: edge->corners | std::views::values) {
                        if (corner2->house != nullptr) {
                            can_build = false;
                            break;
                        }
                    }
                }
                if (can_build) {
                    corner_actor->set_highlighted(true);
                }
            }
        }
    }

    void enable_building_spots(std::vector<Actor *> &actors) {
        for (auto actor: actors) {
            auto *corner_actor = dynamic_cast<CornerActor *>(actor);
            if (corner_actor == nullptr)
                continue;
            auto corner = corner_actor->get_corner();
            if (corner->house == nullptr) {
                bool free_neighbors = true;
                for (auto edge: corner->edges | std::views::values) {
                    for (auto corner2: edge->corners | std::views::values) {
                        if (corner2->house != nullptr) {
                            free_neighbors = false;
                            break;
                        }
                    }
                }
                if (free_neighbors) {
                    if (!corner_actor->get_highlighted()) {
                        corner_actor->set_highlighted(true);
                    }
                }
            }
        }
    }

    void enable_road_spots(std::vector<Actor *> &actors) {
        for (auto actor: actors) {
            auto *edge_actor = dynamic_cast<EdgeActor *>(actor);
            if (edge_actor == nullptr)
                continue;
            auto edge = edge_actor->get_edge();
            if (edge->road == nullptr) {
                bool can_build = false;
                for (auto corner: edge->corners | std::views::values) {
                    if (corner->house != nullptr) {
                        can_build = true;
                        break;
                    }
                    for (auto corner_roads: corner->edges) {
                        if (corner_roads.second->road != nullptr) {
                            can_build = true;
                            break;
                        }
                    }
                    if (can_build)
                        break;
                }
                if (can_build) {
                    if (!edge_actor->get_highlighted()) {
                        edge_actor->set_highlighted(true);
                    }
                }
            }
        }
    }

    void enable_upgrade_spots(const std::vector<Actor *> actors) {
        for (auto actor: actors) {
            auto *corner_actor = dynamic_cast<CornerActor *>(actor);
            if (corner_actor == nullptr)
                continue;
            auto corner = corner_actor->get_corner();
            if (corner->house != nullptr) {
                if (corner->house->level == 1) {
                    corner_actor->set_upgradable(true);
                }
            }
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
        constexpr RenderSettings render_settings{.hex_size = 70, .full_hex_size = 90};

        const RenderResources render_resources = {
            .map_font = LoadFontEx("resources/OpenSans-Regular.ttf", 128, nullptr, 250),
            .resource_sprites =
            {
                .bricks = LoadTexture("resources/sprites/resources/resource_bricks.png"),
                .sheep = LoadTexture("resources/sprites/resources/resource_sheep.png"),
                .wood = LoadTexture("resources/sprites/resources/resource_wood.png"),
                .cactus = LoadTexture("resources/sprites/resources/resource_cactus.png"),
                .stone = LoadTexture("resources/sprites/resources/resource_stone.png"),
                .wheat = LoadTexture("resources/sprites/resources/resource_wheat.png"),
                .random = LoadTexture("resources/sprites/resources/resource_random.png"),
                .resource_outline = LoadTexture("resources/sprites/resources/resource_outline.png"),
                .resource_stack = LoadTexture("resources/sprites/resources/coin_stack.png"),
                .trade = LoadTexture("resources/sprites/resources/trade.png"),
            },
            .sprites =
            {
                .house = LoadTexture("resources/sprites/house.png"),
                .town = LoadTexture("resources/sprites/village.png"),
            },
            .ui = {
                .upgrade = LoadTexture("resources/sprites/ui/upgrade.png"),
            }


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


        std::unordered_map<Map::Resource, int> player_resources = {
            {Map::Resource::WHEAT, 20}, {Map::Resource::WOOD, 20}, {Map::Resource::BRICK, 20},
            {Map::Resource::SHEEP, 20}, {Map::Resource::STONE, 20},
        };

        std::unordered_map<Map::Resource, int> delta_resources = {
            {Map::Resource::WHEAT, 0}, {Map::Resource::WOOD, 0}, {Map::Resource::BRICK, 0},
            {Map::Resource::SHEEP, 0}, {Map::Resource::STONE, 0},
        };

        Map::Map map = Map::Map::build_map_of_size(2);
        std::vector<Roll> rolls;
        for (int i = 1; i < 7; i++) {
            for (int j = 1; j < 7; j++) {
                rolls.emplace_back(i + j);
            }
        }
        std::default_random_engine random_engine{std::random_device{}()};
        std::ranges::shuffle(rolls, random_engine);
        std::deque<Roll> rolls_queue;
        std::vector<Roll> discard_pile;
        rolls_queue.insert(rolls_queue.end(), rolls.begin(), rolls.end());

        std::vector<Actor *> actors;
        AnimationSequence animation_sequence{};
        Game::GameSequence game_sequence;

        game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::PLAYER_TURN));
        game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::ROLL));
        game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::FREE_ROAD));
        game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::FREE_BUILDING));
        game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::FREE_ROAD));
        game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::FREE_BUILDING));

        Game::GamePhaseParameterized do_nothing =
                Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::DO_NOTHING);

        auto *game_phase = &do_nothing;
        float time_waited = 0.0f;
        std::vector<Map::Edge *> edges_to_highlight;
        ContainerActor *drag_and_drop_trade = nullptr;
        Map::Resource dragged_resource = Map::Resource::NONE;

        auto player_resources_container = ResourceDisplayActor(render_resources, player_resources);
        player_resources_container.set_position(
            {-GetScreenWidth() / 2.0f + 100.0f, -GetScreenHeight() / 2.0f + 100.0f});
        player_resources_container.set_background_color(color_scheme.mapBorder);

        actors.push_back(&player_resources_container);

        // Resource Actors:

        for (const auto [coord, edge]: map.get_edges()) {
            const auto hex_position = compute_hex_center_position(render_settings.full_hex_size, coord.hex_coord);
            const auto edge_direction = get_direction_for_edge(coord.edge_direction);
            const auto edge_position_scaling = Vector2{.x = cos(edge_direction), .y = sin(edge_direction)};
            const auto edge_delta = Vector2Scale(edge_position_scaling, render_settings.full_hex_size);
            const auto edge_position = Vector2Add(hex_position, edge_delta);


            actors.push_back(
                new EdgeActor(render_settings, render_resources, color_scheme, edge, coord, edge_position));
        }
        for (auto [coord, corner]: map.get_corners()) {
            const auto hex_position = compute_hex_center_position(render_settings.full_hex_size, coord.hex_coord);
            const auto corner_direction = get_direction_for_corner(coord.corner_direction);
            const auto corner_position_scaling = Vector2{.x = cos(corner_direction), .y = sin(corner_direction)};
            const auto corner_delta = Vector2Scale(corner_position_scaling, render_settings.full_hex_size);
            const auto corner_position = Vector2Add(hex_position, corner_delta);

            actors.push_back(
                new CornerActor(render_settings, render_resources, color_scheme, corner, coord, corner_position));
        }

        while (!WindowShouldClose()) {
            while (game_phase->get_phase() == Game::GamePhase::DO_NOTHING) {
                if (!game_sequence.empty()) {
                    if (auto &action = game_sequence[game_sequence.size() - 1]; action.is_finished()) {
                        game_sequence.pop_back();
                    } else {
                        game_phase = action.get_next_phase();
                        if (game_phase->get_phase() == Game::GamePhase::FREE_BUILDING) {
                            enable_building_spots(actors);
                        } else if (game_phase->get_phase() == Game::GamePhase::WAIT_FOR_TIME) {
                            time_waited = 0.0f;
                        } else if (game_phase->get_phase() == Game::GamePhase::FREE_ROAD) {
                            for (auto actor: actors) {
                                auto *edge_actor = dynamic_cast<EdgeActor *>(actor);
                                if (edge_actor == nullptr)
                                    continue;
                                for (auto edge: edges_to_highlight) {
                                    if (edge_actor->get_edge() == edge) {
                                        if (edge->road == nullptr)
                                            edge_actor->set_highlighted(true);
                                    }
                                }
                            }
                        } else if (game_phase->get_phase() == Game::GamePhase::PLAYER_TURN) {
                            if (player_resources[Map::Resource::WOOD] >= 1 &&
                                player_resources[Map::Resource::BRICK] >= 1) {
                                enable_road_spots(actors);
                                if (player_resources[Map::Resource::WHEAT] >= 1 &&
                                    player_resources[Map::Resource::SHEEP] >= 1) {
                                    enable_building_spots_with_roads(actors);
                                }
                            }

                            if (player_resources[Map::Resource::STONE] >= 3 && player_resources[Map::Resource::WHEAT] >=
                                2) {
                                enable_upgrade_spots(actors);
                            }
                        }
                    }
                } else {
                    break;
                }
            }
            if (game_phase->get_phase() == Game::GamePhase::FREE_BUILDING) {
                // get clicks
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);
                    std::cout << mouse_position.x << " " << mouse_position.y << "\n";
                    for (auto actor: actors) {
                        auto *corner_actor = dynamic_cast<CornerActor *>(actor);
                        if (corner_actor == nullptr)
                            continue;
                        if (!corner_actor->is_clicked(mouse_position))
                            continue;
                        if (!corner_actor->get_highlighted()) {
                            // display error message
                            break;
                        }
                        auto *corner = corner_actor->get_corner();
                        edges_to_highlight.clear();
                        for (auto edge: corner->edges | std::views::values) {
                            edges_to_highlight.push_back(edge);
                        }
                        corner_actor->set_house(new House());
                        corner_actor->set_base_color(GREEN);
                        for (auto actor2: actors) {
                            auto *corner_actor2 = dynamic_cast<CornerActor *>(actor2);
                            if (corner_actor2 == nullptr)
                                continue;
                            corner_actor2->set_highlighted(false);
                        }
                        game_phase = &do_nothing;
                        break;
                    }
                }
            } else if (game_phase->get_phase() == Game::GamePhase::WAIT_FOR_TIME) {
                time_waited += GetFrameTime();
                if (time_waited >= game_phase->get_wait_for_time_parameter()->time_in_s) {
                    game_phase->cleanup();
                    game_phase = &do_nothing;
                    time_waited = 0.0f;
                }
            } else if (game_phase->get_phase() == Game::GamePhase::FREE_ROAD) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);
                    std::cout << mouse_position.x << " " << mouse_position.y << "\n";
                    for (auto actor: actors) {
                        auto *edge_actor = dynamic_cast<EdgeActor *>(actor);
                        if (edge_actor == nullptr)
                            continue;
                        if (!edge_actor->is_clicked(mouse_position))
                            continue;
                        if (!edge_actor->get_highlighted()) {
                            // display error message
                            break;
                        }
                        edge_actor->set_road(new Road());
                        for (auto actor2: actors) {
                            auto *edge_actor2 = dynamic_cast<EdgeActor *>(actor2);
                            if (edge_actor2 == nullptr)
                                continue;
                            edge_actor2->set_highlighted(false);
                        }
                        for (auto actor2: actors) {
                            auto *corner_actor2 = dynamic_cast<CornerActor *>(actor2);
                            if (corner_actor2 != nullptr) {
                                corner_actor2->set_highlighted(false);
                                auto corner = corner_actor2->get_corner();
                                if (corner->house == nullptr) {
                                    int corner_roads_built = 0;
                                    for (auto edge: corner->edges | std::views::values) {
                                        if (edge->road != nullptr) {
                                            corner_roads_built++;
                                        }
                                    }
                                    if (corner_roads_built >= 2) {
                                        corner_actor2->set_base_color(GREEN);
                                    }
                                }
                            }
                        }
                        game_phase = &do_nothing;
                        break;
                    }
                }
            } else if (game_phase->get_phase() == Game::GamePhase::ROLL) {
                if (IsKeyReleased(KEY_SPACE)) {
                    if (rolls_queue.empty()) {
                        game_phase = &do_nothing;
                    } else {
                        Roll roll = rolls_queue.front();
                        discard_pile.push_back(roll);
                        rolls_queue.pop_front();
                        if (rolls_queue.size() < 5) {
                            std::ranges::shuffle(discard_pile, random_engine);
                            rolls_queue.insert(rolls_queue.end(), discard_pile.begin(), discard_pile.end());
                            discard_pile.clear();
                        }
                        for (const auto hex: map.get_hexes() | std::views::values) {
                            if (hex->number == roll.get_roll() && hex->resource != Map::Resource::NONE) {
                                for (const auto corner: hex->corners | std::views::values) {
                                    if (corner->house != nullptr) {
                                        player_resources[hex->resource] =
                                                player_resources[hex->resource] + corner->house->level;
                                    }
                                }
                            }
                        }
                        game_phase = &do_nothing;
                    }
                }
            } else if (game_phase->get_phase() == Game::GamePhase::PLAYER_TURN) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);

                    bool build = false;

                    for (auto actor: actors) {
                        auto *corner_actor = dynamic_cast<CornerActor *>(actor);
                        if (corner_actor == nullptr)
                            continue;
                        if (!corner_actor->is_clicked(mouse_position))
                            continue;
                        if (corner_actor->get_highlighted()) {
                            // display error message

                            auto *corner = corner_actor->get_corner();
                            edges_to_highlight.clear();
                            for (auto edge: corner->edges | std::views::values) {
                                edges_to_highlight.push_back(edge);
                            }
                            corner_actor->set_house(new House());
                            corner_actor->set_base_color(GREEN);
                            player_resources[Map::Resource::WOOD] = player_resources[Map::Resource::WOOD] - 1;
                            player_resources[Map::Resource::BRICK] = player_resources[Map::Resource::BRICK] - 1;
                            player_resources[Map::Resource::SHEEP] = player_resources[Map::Resource::SHEEP] - 1;
                            player_resources[Map::Resource::WHEAT] = player_resources[Map::Resource::WHEAT] - 1;
                            build = true;
                        } else if (corner_actor->get_is_upgradable()) {
                            corner_actor->get_corner()->house->level++;
                            player_resources[Map::Resource::STONE] = player_resources[Map::Resource::STONE] - 3;
                            player_resources[Map::Resource::WHEAT] = player_resources[Map::Resource::WHEAT] - 2;
                            build = true;
                        }

                        break;
                    }

                    for (auto actor: actors) {
                        auto *edge_actor = dynamic_cast<EdgeActor *>(actor);
                        if (edge_actor == nullptr)
                            continue;
                        if (!edge_actor->is_clicked(mouse_position))
                            continue;
                        if (!edge_actor->get_highlighted()) {
                            // display error message
                            break;
                        }
                        edge_actor->set_road(new Road());

                        player_resources[Map::Resource::WOOD] = player_resources[Map::Resource::WOOD] - 1;
                        player_resources[Map::Resource::BRICK] = player_resources[Map::Resource::BRICK] - 1;
                        build = true;
                        break;
                    }
                    if (build) {
                        for (auto actor2: actors) {
                            auto *edge_actor2 = dynamic_cast<EdgeActor *>(actor2);
                            if (edge_actor2 != nullptr) {
                                edge_actor2->set_highlighted(false);
                                continue;
                            }
                            auto *corner_actor2 = dynamic_cast<CornerActor *>(actor2);
                            if (corner_actor2 != nullptr) {
                                corner_actor2->set_highlighted(false);
                                corner_actor2->set_upgradable(false);
                                auto corner = corner_actor2->get_corner();
                                if (corner->house == nullptr) {
                                    int corner_roads_built = 0;
                                    for (auto edge: corner->edges | std::views::values) {
                                        if (edge->road != nullptr) {
                                            corner_roads_built++;
                                        }
                                    }
                                    if (corner_roads_built >= 2) {
                                        corner_actor2->set_base_color(GREEN);
                                    }
                                }
                            }
                        }


                        if (player_resources[Map::Resource::WOOD] >= 1 && player_resources[Map::Resource::BRICK] >= 1) {
                            enable_road_spots(actors);
                            if (player_resources[Map::Resource::WHEAT] >= 1 &&
                                player_resources[Map::Resource::SHEEP] >= 1) {
                                enable_building_spots_with_roads(actors);
                            }
                        }
                        if (player_resources[Map::Resource::STONE] >= 3 && player_resources[Map::Resource::WHEAT] >=
                            2) {
                            enable_upgrade_spots(actors);
                        }
                    }
                    if (drag_and_drop_trade == nullptr) {
                        auto resource = player_resources_container.is_over_resource_sprite(mouse_position);
                        if (resource.has_value()) {
                            if (player_resources[resource.value()] >= 4) {
                                dragged_resource = resource.value();
                                drag_and_drop_trade = player_resources_container.get_drag_and_drop_resource(
                                    render_resources, mouse_position);
                            }
                        }
                    }
                } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);
                    if (drag_and_drop_trade != nullptr) {
                        drag_and_drop_trade->set_position(mouse_position);
                        drag_and_drop_trade->render();
                        for (const auto resource: delta_resources | std::views::keys) {
                            delta_resources[resource] = 0;
                        }
                        delta_resources[dragged_resource] = -4;
                        auto traded_resource = player_resources_container.is_over_resource(mouse_position);
                        if (traded_resource.has_value() && traded_resource.value() != dragged_resource) {
                            delta_resources[traded_resource.value()] = 1;
                        }
                    }
                } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    if (drag_and_drop_trade != nullptr) {
                        Vector2 mouse_position = GetScreenToWorld2D(GetMousePosition(), camera);
                        auto traded_resource = player_resources_container.is_over_resource(mouse_position);

                        if (traded_resource.has_value() && traded_resource.value() != dragged_resource) {
                            player_resources[traded_resource.value()] += 1;
                            player_resources[dragged_resource] -= 4;
                            dragged_resource = Map::Resource::NONE;
                        }

                        for (const auto resource: delta_resources | std::views::keys) {
                            delta_resources[resource] = 0;
                        }


                        if (player_resources[Map::Resource::WOOD] >= 1 && player_resources[Map::Resource::BRICK] >= 1) {
                            enable_road_spots(actors);
                            if (player_resources[Map::Resource::WHEAT] >= 1 &&
                                player_resources[Map::Resource::SHEEP] >= 1) {
                                enable_building_spots_with_roads(actors);
                            }
                        }
                        if (player_resources[Map::Resource::STONE] >= 3 && player_resources[Map::Resource::WHEAT] >=
                            2) {
                            enable_upgrade_spots(actors);
                        }

                        drag_and_drop_trade->cleanup();
                        delete drag_and_drop_trade;
                        drag_and_drop_trade = nullptr;
                    }
                } else if (IsKeyReleased(KEY_SPACE)) {
                    for (auto actor2: actors) {
                        auto *edge_actor2 = dynamic_cast<EdgeActor *>(actor2);
                        if (edge_actor2 != nullptr)
                            edge_actor2->set_highlighted(false);
                        auto *corner_actor2 = dynamic_cast<CornerActor *>(actor2);
                        if (corner_actor2 != nullptr)
                            corner_actor2->set_highlighted(false);
                    }
                    game_sequence.emplace_back(
                        Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::PLAYER_TURN));
                    game_sequence.emplace_back(Game::GamePhaseParameterized::for_no_parameter(Game::GamePhase::ROLL));
                    game_sequence.emplace_back(Game::GamePhaseParameterized::wait_for_time(0.2));
                    game_phase = &do_nothing;
                }
            }
            float delta_time = GetFrameTime();
            for (auto actor: actors) {
                actor->update(delta_time);
            }
            animation_sequence.tick_animation(delta_time);
            player_resources_container.update_resources(player_resources, delta_resources);
            // render phase
            BeginDrawing();
            ClearBackground({51, 51, 51, 255});
            BeginMode2D(camera);
            render_map(render_settings, render_resources, color_scheme, map);
            for (const auto &actor: actors) {
                actor->render();
            }
            if (drag_and_drop_trade != nullptr)
                drag_and_drop_trade->render();
            EndMode2D();

            // DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), RED);
            // DrawLine(0.0, GetScreenHeight() / 2, GetScreenWidth(), GetScreenHeight() / 2, RED);
            // DrawLineEx({0.0, screen_height - 5}, {screen_width, screen_height - 5}, 10, RED);
            DrawTextEx(render_resources.map_font, std::to_string(GetFPS()).c_str(), {30.0, 30.0}, 32.0, 2.0, WHITE);
            render_rolls(render_resources, rolls_queue);

            if (game_phase->get_phase() == Game::GamePhase::ROLL) {
                std::string text = "Press space to roll";
                Vector2 center_bottom = {
                    (GetScreenWidth() / 2.0f),
                    static_cast<float>(GetScreenHeight() - 100)
                };
                Vector2 text_size = MeasureTextEx(render_resources.map_font, text.c_str(), 32.0, 0.0f);
                Vector2 text_position =
                        Vector2Subtract(center_bottom, Vector2Divide(text_size, {.x = 2.0f, .y = 2.0f}));
                DrawTextEx(render_resources.map_font, text.c_str(), text_position, 32.0, 0.0f, WHITE);
            } else if (game_phase->get_phase() == Game::GamePhase::PLAYER_TURN) {
                std::string text = "Press space to end turn";
                Vector2 center_bottom = {
                    (static_cast<float>(GetScreenWidth()) / 2.0f),
                    static_cast<float>(GetScreenHeight() - 100)
                };
                Vector2 text_size = MeasureTextEx(render_resources.map_font, text.c_str(), 32.0, 0.0f);
                Vector2 text_position =
                        Vector2Subtract(center_bottom, Vector2Divide(text_size, {.x = 2.0f, .y = 2.0f}));
                DrawTextEx(render_resources.map_font, text.c_str(), text_position, 32.0, 0.0f, WHITE);
            }

            EndDrawing();
        }

        UnloadFont(render_resources.map_font);

        CloseWindow();
    } // namespace Engine
} // namespace Engine
