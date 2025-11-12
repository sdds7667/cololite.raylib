
#include "coords.hh"
#include <ostream>
#include <stdexcept>
#include <string>

namespace Map {

HexEdgeDirection opposite_direction(const HexEdgeDirection &direction) {
  return static_cast<HexEdgeDirection>(static_cast<int>(direction) + 3 % 6);
}

HexCornerDirection opposite_direction(const HexCornerDirection &direction) {
  return static_cast<HexCornerDirection>(static_cast<int>(direction) + 3 % 6);
}

CornerEdgeDirection opposite_direction(const CornerEdgeDirection &direction) {
  return static_cast<CornerEdgeDirection>(static_cast<int>(direction) + 3 % 6);
}

HexCornerDirection edge_direction_to_corner_direction(
    const HexEdgeDirection &edge_direction,
    const CornerEdgeDirection &edge_corner_direction) {
#define CORNER_EDGE_TO_HEX_CORNER(direction1, corner_direction1, direction2,   \
                                  corner_direction2)                           \
  switch (edge_corner_direction) {                                             \
  case direction1:                                                             \
    return corner_direction1;                                                  \
  case direction2:                                                             \
    return corner_direction2;                                                  \
  default:                                                                     \
    throw std::runtime_error("This edge " + std::to_string(edge_direction) +   \
                             "does not support this corner." +                 \
                             std::to_string(edge_corner_direction));           \
    break;                                                                     \
  }

  switch (edge_direction) {
  case HexEdgeDirection::BOTTOM_RIGHT:
    CORNER_EDGE_TO_HEX_CORNER(
        CornerEdgeDirection::BOTTOM_LEFT, HexCornerDirection::BOTTOM_RIGHT,
        CornerEdgeDirection::TOP_RIGHT, HexCornerDirection::RIGHT);
  case HexEdgeDirection::BOTTOM:
    CORNER_EDGE_TO_HEX_CORNER(
        CornerEdgeDirection::LEFT, HexCornerDirection::BOTTOM_LEFT,
        CornerEdgeDirection::RIGHT, HexCornerDirection::BOTTOM_RIGHT);
  case HexEdgeDirection::BOTTOM_LEFT:
    CORNER_EDGE_TO_HEX_CORNER(
        CornerEdgeDirection::BOTTOM_RIGHT, HexCornerDirection::BOTTOM_LEFT,
        CornerEdgeDirection::TOP_LEFT, HexCornerDirection::LEFT);
  case HexEdgeDirection::TOP_LEFT:
    CORNER_EDGE_TO_HEX_CORNER(
        CornerEdgeDirection::BOTTOM_LEFT, HexCornerDirection::LEFT,
        CornerEdgeDirection::TOP_RIGHT, HexCornerDirection::TOP_LEFT);
  case HexEdgeDirection::TOP:
    CORNER_EDGE_TO_HEX_CORNER(
        CornerEdgeDirection::LEFT, HexCornerDirection::TOP_LEFT,
        CornerEdgeDirection::RIGHT, HexCornerDirection::TOP_RIGHT);
  case HexEdgeDirection::TOP_RIGHT:
    CORNER_EDGE_TO_HEX_CORNER(
        CornerEdgeDirection::BOTTOM_RIGHT, HexCornerDirection::RIGHT,
        CornerEdgeDirection::TOP_LEFT, HexCornerDirection::TOP_RIGHT);
  }
#undef CORNER_EDGE_TO_HEX_CORNER
};

bool HexCoord2::operator==(const HexCoord2 &rhs) const {
  return q == rhs.q && r == rhs.r;
}

HexCoord2 HexCoord2::operator+(const HexCoord2 &rhs) const {
  return {q + rhs.q, r + rhs.r};
}

HexCoord2 HexCoord2::get_neighbouring_hex_coord(
    HexEdgeDirection neighbour_direction) const {
  switch (neighbour_direction) {
  case HexEdgeDirection::TOP:
    return {q, r - 1};
  case HexEdgeDirection::TOP_RIGHT:
    return {q + 1, r - 1};
  case HexEdgeDirection::BOTTOM_RIGHT:
    return {q + 1, r};
  case HexEdgeDirection::BOTTOM:
    return {q, r + 1};
  case HexEdgeDirection::BOTTOM_LEFT:
    return {q - 1, r + 1};
  case HexEdgeDirection::TOP_LEFT:
    return {q - 1, r};
  }
}

EdgeCoord HexCoord2::get_edge_coord(HexEdgeDirection edge_direction) const {
  return EdgeCoord{*this, edge_direction};
}

CornerCoord
HexCoord2::get_corner_coord(HexCornerDirection corner_direction) const {
  return CornerCoord{*this, corner_direction};
}

bool EdgeCoord::operator==(const EdgeCoord &other) const {
  return edge_direction == other.edge_direction && hex_coord == other.hex_coord;
}

bool CornerCoord::operator==(const CornerCoord &other) const {
  return corner_direction == other.corner_direction &&
         hex_coord == other.hex_coord;
}

} // namespace Map

namespace std {
std::string to_string(const Map::HexCoord2 &coord) {
  return "HexCoord2<q=" + std::to_string(coord.q) + ", " +
         "r = " + std::to_string(coord.r) + ">";
};

std::string to_string(const Map::HexEdgeDirection &edge_direction) {
  const std::string index = std::to_string(static_cast<int>(edge_direction));
  switch (edge_direction) {
  case Map::HexEdgeDirection::BOTTOM_RIGHT:
    return "EdgeDir<BOTTOM_RIGHT = " + index + ">";
  case Map::HexEdgeDirection::BOTTOM:
    return "EdgeDir<BOTTOM = " + index + ">";
  case Map::HexEdgeDirection::BOTTOM_LEFT:
    return "EdgeDir<BOTTOM_LEFT = " + index + ">";
  case Map::HexEdgeDirection::TOP_LEFT:
    return "EdgeDir<TOP_LEFT = " + index + ">";
  case Map::HexEdgeDirection::TOP:
    return "EdgeDir<TOP = " + index + ">";
  case Map::HexEdgeDirection::TOP_RIGHT:
    return "EdgeDir<TOP_RIGHT = " + index + ">";
  }
}

std::string to_string(const Map::EdgeCoord &edge_coord) {
  return "EdgeCoord<hex_coord=" + std::to_string(edge_coord.hex_coord) +
         ", edge_direction=" + std::to_string(edge_coord.edge_direction) + ">";
};

std::string to_string(const Map::HexCornerDirection &corner_direction) {
  const std::string index = std::to_string(static_cast<int>(corner_direction));
  switch (corner_direction) {
  case Map::HexCornerDirection::BOTTOM_RIGHT:
    return "CornerDir<BOTTOM_RIGHT = " + index + ">";
  case Map::HexCornerDirection::BOTTOM_LEFT:
    return "CornerDir<BOTTOM_LEFT = " + index + ">";
  case Map::HexCornerDirection::LEFT:
    return "CornerDir<LEFT = " + index + ">";
  case Map::HexCornerDirection::TOP_LEFT:
    return "CornerDir<TOP_LEFT = " + index + ">";
  case Map::HexCornerDirection::TOP_RIGHT:
    return "CornerDir<TOP_RIGHT = " + index + ">";
  case Map::HexCornerDirection::RIGHT:
    return "CornerDir<RIGHT = " + index + ">";
  }
}

std::string to_string(const Map::CornerCoord &corner_coord) {
  return "CornerCoord<hex_coord=" + std::to_string(corner_coord.hex_coord) +
         ", corner_direction=" + std::to_string(corner_coord.corner_direction) +
         ">";
};

string to_string(const Map::CornerEdgeDirection &corner_edge_direction) {
  switch (corner_edge_direction) {
  case Map::CornerEdgeDirection::RIGHT:
    return "CornerEdgeDirection::RIGHT";
  case Map::CornerEdgeDirection::BOTTOM_RIGHT:
    return "CornerEdgeDirection::BOTTOM_RIGHT";
  case Map::CornerEdgeDirection::BOTTOM_LEFT:
    return "CornerEdgeDirection::BOTTOM_LEFT";
  case Map::CornerEdgeDirection::LEFT:
    return "CornerEdgeDirection::LEFT";
  case Map::CornerEdgeDirection::TOP_LEFT:
    return "CornerEdgeDirection::TOP_LEFT";
  case Map::CornerEdgeDirection::TOP_RIGHT:
    return "CornerEdgeDirection::TOP_RIGHT";
  }
}

} // namespace std

std::ostream &operator<<(std::ostream &output_stream,
                         const Map::HexCoord2 &coord) {
  return output_stream << std::to_string(coord);
};

std::ostream &operator<<(std::ostream &output_stream,
                         const Map::HexCornerDirection &corner_direction) {
  return output_stream << std::to_string(corner_direction);
};

std::ostream &operator<<(std::ostream &output_stream,
                         const Map::HexEdgeDirection &edge_direction) {
  return output_stream << std::to_string(edge_direction);
};

std::ostream &operator<<(std::ostream &output_stream,
                         const Map::EdgeCoord &edge_coord) {
  return output_stream << std::to_string(edge_coord);
};

std::ostream &operator<<(std::ostream &output_stream,
                         const Map::CornerCoord &corner_coord) {
  return output_stream << std::to_string(corner_coord);
};
