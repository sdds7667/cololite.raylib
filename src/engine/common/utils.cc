//
// Created by Ion Plamadeala on 05/12/2025.
//
#include "utils.hh"

#include <numbers>

#include "engine_settings.hh"

namespace Engine::Utils {
    Vector2 compute_hex_center_position(const float hex_size, const Map::HexCoord2 &hex_coord) {
        return {
            .x = hex_size * 3.0f / 2.0f * static_cast<float>(hex_coord.q),
            .y = hex_size *
                 (std::numbers::sqrt3_v<float> / 2.0f * static_cast<float>(hex_coord.q) + std::numbers::sqrt3_v<float>)
        };
    }

    Vector2 compute_hex_center_position(const Map::HexCoord2 &hex_coord) {
        const auto hex_size = get_engine_settings().get_render_settings().full_hex_size;

        return compute_hex_center_position(hex_size, hex_coord);
    }
}

