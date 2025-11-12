#pragma once

#include "coords.hh"

template <> struct std::hash<Map::HexCoord2> {
  size_t operator()(const Map::HexCoord2 &s) const noexcept {
    size_t seed = 0;
    seed ^= std::hash<int>{}(s.q) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= std::hash<int>{}(s.r) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};

template <> struct std::hash<Map::HexCornerDirection> {
  size_t operator()(const Map::HexCornerDirection &s) const noexcept {
    size_t seed = 0;
    seed ^= std::hash<int>{}(static_cast<int>(s)) + 0x9e3779b9 + (seed << 6) +
            (seed >> 2);
    return seed;
  }
};

template <> struct std::hash<Map::EdgeCoord> {
  size_t operator()(const Map::EdgeCoord &s) const noexcept {
    size_t seed = 0;
    seed ^= std::hash<Map::HexCoord2>{}(s.hex_coord) + 0x9e3779b9 +
            (seed << 6) + (seed >> 2);
    seed ^= std::hash<Map::HexEdgeDirection>{}(s.edge_direction) + 0x9e3779b9 +
            (seed << 6) + (seed >> 2);
    return seed;
  }
};

template <> struct std::hash<Map::CornerCoord> {
  size_t operator()(const Map::CornerCoord &s) const noexcept {
    size_t seed = 0;
    seed ^= std::hash<Map::HexCoord2>{}(s.hex_coord) + 0x9e3779b9 +
            (seed << 6) + (seed >> 2);
    seed ^= std::hash<Map::HexCornerDirection>{}(s.corner_direction) +
            0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};
