//
// Created by Ion Plamadeala on 05/12/2025.
//

#ifndef COLOLITE_UTILS_HH
#define COLOLITE_UTILS_HH

#include "coords.hh"
#include "raylib.h"

namespace Engine::Utils {
    Vector2 compute_hex_center_position(const float hex_size, const Map::HexCoord2 &hex_coord);

    Vector2 compute_hex_center_position(const Map::HexCoord2 &hex_coord);
}

#endif //COLOLITE_UTILS_HH
