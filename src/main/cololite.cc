
#include "corner_actor.hh"
#include "edge_actor.hh"
#include "engine_core.hh"
#include "engine_settings.hh"
#include "game_state.hh"
#include "map_actor.hh"
#include "raylib.h"
#include "raymath.h"
#include "scene.hh"
#include "utils.hh"

auto get_direction_for_corner(const Map::HexCornerDirection &corner_direction) -> float {
    constexpr float sixty_degrees = PI / 3.0f;
    return sixty_degrees * static_cast<float>(corner_direction);
}

auto get_direction_for_edge(const Map::HexEdgeDirection &edge_direction) -> float {
    constexpr float sixty_degrees = PI / 3.0f;
    return static_cast<float>(edge_direction) * sixty_degrees;
}

auto main() -> int {
    const auto &engine_settings = get_engine_settings();
    auto &game_state = Game::get_game_state();
    GameActors::MapActor map_actor(Vector2Zero(), game_state.get_map());
    auto &render_settings = engine_settings.get_render_settings();
    for (const auto [coord, edge]: Game::get_game_state().get_map().get_edges()) {
        const auto hex_position =
                Utils::compute_hex_center_position(coord.hex_coord);
        const auto edge_direction = get_direction_for_edge(coord.edge_direction);
        const auto edge_position_scaling = Vector2{.x = cos(edge_direction), .y = sin(edge_direction)};
        const auto edge_delta = Vector2Scale(edge_position_scaling,
                                             engine_settings.get_render_settings().full_hex_size);
        const auto edge_position = Vector2Add(hex_position, edge_delta);


        map_actor.get_children().add(new GameActors::EdgeActor(edge, coord, edge_position), RenderLayer::MAP_EDGES);
    }
    for (auto [coord, corner]: game_state.get_map().get_corners()) {
        const auto hex_position =
                Utils::compute_hex_center_position(coord.hex_coord);
        const auto corner_direction = get_direction_for_corner(coord.corner_direction);
        const auto corner_position_scaling = Vector2{.x = cos(corner_direction), .y = sin(corner_direction)};
        const auto corner_delta = Vector2Scale(corner_position_scaling, render_settings.full_hex_size);
        const auto corner_position = Vector2Add(hex_position, corner_delta);

        map_actor.get_children().add(new GameActors::CornerActor(corner, coord, corner_position),
                                     RenderLayer::MAP_CORNERS);
    }
    Engine::initialize();
    Engine::Scene main_scene;
    Engine::get_engine_settings().set_scene(&main_scene);

    while (!WindowShouldClose()) {
        Engine::update(GetFrameTime());
        Engine::render();
    }
    Engine::terminate();
    return 0;
}
