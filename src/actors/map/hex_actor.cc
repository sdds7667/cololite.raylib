//
// Created by Ion Plamadeala on 05/12/2025.
//

#include "hex_actor.hh"
#include <numbers>
#include "raymath.h"
#include "utils.hh"

constexpr float sqrt3 = std::numbers::sqrt3_v<float>;

GameActors::HexActor::HexActor(const Vector2 position, const float hex_radius,
                               Map::Hex *hex) : PositionedActor(position),
                                                m_hex_radius(hex_radius), m_hex(hex) {
}

void GameActors::HexActor::update(float deltaTime) {
}


void GameActors::HexActor::render() const {
    const auto &engine_settings = get_engine_settings();
    const auto &color_scheme = engine_settings.get_color_scheme();
    const auto &render_settings = engine_settings.get_render_settings();
    const auto &render_resources = engine_settings.get_render_resources();

    constexpr Vector2 divide_by_two{.x = 2.0, .y = 2.0};

    for (const auto coord2: Map::MapCoords(2)) {
        Color offset = get_color_for_resource(m_hex->resource);
        const int q = coord2.q;
        const int r = coord2.r;
        const int distance = (std::abs(coord2.q) + std::abs(coord2.q + coord2.r) + std::abs(coord2.r)) / 2;
        if (!((q == -2 && r <= 1) || (q == -1 && r <= 0) || (q == 0 && r <= 0))) {
            constexpr int factor = 12;
            offset.r -= distance * factor;
            offset.g -= distance * factor;
            offset.b -= distance * factor;
        }

        const float little_size = render_settings.hex_size / 3.5f;
        const auto little_hex_position = Utils::compute_hex_center_position(little_size, coord2);
        DrawPoly(Vector2Add(get_position(), little_hex_position), 6, little_size, 0, offset);
    }
    const auto texture = get_texture_for_resource(m_hex->resource);
    const float scale = render_settings.hex_size * (2.5f / 3.0f) / static_cast<float>(texture.width);

    const Vector2 sprite_center_position = {
        .x = get_position().x - render_settings.hex_size * 1.0f / 3.0f,
        .y = get_position().y - render_settings.hex_size * 1.0f / 3.0f
    };

    const Vector2 sprite_position = {
        .x = sprite_center_position.x - scale * static_cast<float>(texture.width) / 2.0f,
        .y = sprite_center_position.y - scale * static_cast<float>(texture.height) / 2.0f
    };

    DrawTextureEx(texture, sprite_position, 0.0f, scale, WHITE);

    const Vector2 circle_position = {
        .x = get_position().x + render_settings.hex_size * 1.0f / 3.0f,
        .y = get_position().y + render_settings.hex_size * 1.0f / 3.0f,
    };

    DrawCircleV(circle_position, render_settings.hex_size * 1.0f / 3.0f, color_scheme.mapBorder);

    const std::string number = std::to_string(m_hex->number);
    const auto font_size = MeasureTextEx(render_resources.map_font, number.c_str(), 32.0, 0.0f);
    const Vector2 final_text_position =
            Vector2Subtract(circle_position, Vector2Divide(font_size, divide_by_two));
    DrawTextEx(render_resources.map_font, number.c_str(), final_text_position, 32.0f, 0, WHITE);
    // DrawText(number.c_str(), final_text_position.x, final_text_position.y, 32, WHITE);
}
