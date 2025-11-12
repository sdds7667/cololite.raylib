
#include "map.hh"
#include "coords.hh"
#include <unordered_map>
#include <utility>

namespace Map {
  bool MapBounds::is_within_bounds(const HexCoord2 &coord) const {
    return coord.r > min_bounds.r && coord.q > min_bounds.q &&
           coord.r < max_bounds.r && coord.q < max_bounds.q;
  }

  MapBounds MapBounds::from_radius(std::size_t radius) {
    const int bounds = static_cast<int>(radius) - 1;
    return {
      .min_bounds = {.q = -bounds, .r = -bounds},
      .max_bounds = {.q = +bounds, .r = +bounds}
    };
  };

  CornerCoord
  Map::get_normalized_corner_coord(const CornerCoord &raw_coord) const {
    HexCoord2 potential_coord{};
    HexCoord2 normalized_coord = raw_coord.hex_coord;
    bool assigned = false;
    HexCornerDirection normalized_corner_direction = raw_coord.corner_direction;
#define ASSIGN_CORNER_COORD_IF_POSSIBLE(other_hex_coord,                       \
  final_corner_direction)                \
  potential_coord =                                                            \
      raw_coord.hex_coord.get_neighbouring_hex_coord(other_hex_coord);         \
  if (map_bounds.is_within_bounds(potential_coord)) {                          \
    normalized_coord = potential_coord;                                        \
    normalized_corner_direction = final_corner_direction;                      \
    assigned = true;                                                           \
  }

    switch (raw_coord.corner_direction) {
      case HexCornerDirection::RIGHT:
      case HexCornerDirection::BOTTOM_RIGHT:
        break;
      case HexCornerDirection::BOTTOM_LEFT:
        // can be normalized as RIGHT of BOTTOM_LEFT hex
        ASSIGN_CORNER_COORD_IF_POSSIBLE(HexEdgeDirection::BOTTOM_LEFT,
                                        HexCornerDirection::RIGHT);
        break;
      case HexCornerDirection::LEFT:
        ASSIGN_CORNER_COORD_IF_POSSIBLE(HexEdgeDirection::TOP_LEFT,
                                        HexCornerDirection::BOTTOM_RIGHT);
        break;
      case HexCornerDirection::TOP_LEFT:
        ASSIGN_CORNER_COORD_IF_POSSIBLE(HexEdgeDirection::TOP_LEFT,
                                        HexCornerDirection::RIGHT);
        if (!assigned) {
          ASSIGN_CORNER_COORD_IF_POSSIBLE(HexEdgeDirection::TOP,
                                          HexCornerDirection::BOTTOM_LEFT);
        }
        break;
      case HexCornerDirection::TOP_RIGHT:
        ASSIGN_CORNER_COORD_IF_POSSIBLE(HexEdgeDirection::TOP,
                                        HexCornerDirection::BOTTOM_RIGHT);
        if (!assigned) {
          ASSIGN_CORNER_COORD_IF_POSSIBLE(HexEdgeDirection::TOP,
                                          HexCornerDirection::BOTTOM_LEFT);
        }
        break;
    }
#undef ASSIGN_CORNER_COORD_IF_POSSIBLE
    return {
      .hex_coord = normalized_coord,
      .corner_direction = normalized_corner_direction
    };
  };

  EdgeCoord Map::get_normalized_edge_coord(const EdgeCoord &raw_coord) const {
    HexCoord2 potential_coord;
    HexCoord2 normalized_coord = raw_coord.hex_coord;
    HexEdgeDirection normalized_edge_direction = raw_coord.edge_direction;
#define ASSIGN_EDGE_COORD_IF_POSSIBLE(other_hex_coord, final_edge_direction)   \
  potential_coord =                                                            \
      raw_coord.hex_coord.get_neighbouring_hex_coord(other_hex_coord);         \
  if (map_bounds.is_within_bounds(potential_coord)) {                          \
    normalized_coord = potential_coord;                                        \
    normalized_edge_direction = final_edge_direction;                          \
  }

    switch (raw_coord.edge_direction) {
      case HexEdgeDirection::BOTTOM_RIGHT:
      case HexEdgeDirection::BOTTOM:
      case HexEdgeDirection::BOTTOM_LEFT:
        break;
      case HexEdgeDirection::TOP_LEFT:
        ASSIGN_EDGE_COORD_IF_POSSIBLE(HexEdgeDirection::TOP_LEFT,
                                      HexEdgeDirection::BOTTOM_RIGHT);
        break;
      case HexEdgeDirection::TOP:
        ASSIGN_EDGE_COORD_IF_POSSIBLE(HexEdgeDirection::TOP,
                                      HexEdgeDirection::BOTTOM);
        break;
      case HexEdgeDirection::TOP_RIGHT:
        ASSIGN_EDGE_COORD_IF_POSSIBLE(HexEdgeDirection::TOP_RIGHT,
                                      HexEdgeDirection::BOTTOM_LEFT);
    }
#undef ASSIGN_EDGE_COORD
    return {
      .hex_coord = normalized_coord,
      .edge_direction = normalized_edge_direction
    };
  }

  MapCoords::MapCoords(int size) : m_size(size) {
  }

  MapCoords::MapCoordsIterator::MapCoordsIterator(int size)
    : size(size), r(-size), max_r(size), q(-size + abs(std::min(r, 0))),
      max_q(size - std::max(0, r)), current_coord{.q = q, .r = r} {
  }

  MapCoords::MapCoordsIterator::MapCoordsIterator(int size, int r, int q)
    : size(size), r(r), max_r(size), q(q), max_q(size - std::max(0, r)),
      current_coord{r, q} {
  }

  MapCoords::MapCoordsIterator::reference
  MapCoords::MapCoordsIterator::operator*() const {
    return current_coord;
  }

  MapCoords::MapCoordsIterator::pointer
  MapCoords::MapCoordsIterator::operator->() const {
    return &current_coord;
  }

  MapCoords::MapCoordsIterator &MapCoords::MapCoordsIterator::operator++() {
    if (q > max_q)
      return *this;
    q++;
    if (q > max_q) {
      if (r >= max_r) {
        return *this;
      }
      r++;
      q = -size + abs(std::min(r, 0));
      max_q = size - std::max(0, r);
    }

    current_coord.q = q;
    current_coord.r = r;
    return *this;
  }

  MapCoords::MapCoordsIterator MapCoords::MapCoordsIterator::operator++(int) {
    MapCoordsIterator tmp{size, r, q};
    current_coord.q = q;
    current_coord.r = r;
    ++(*this);
    return tmp;
  }

  MapCoords::MapCoordsIterator MapCoords::begin() const {
    return MapCoordsIterator(m_size);
  }

  MapCoords::MapCoordsIterator MapCoords::end() const {
    return {m_size, m_size, 1};
  }

  bool operator==(const MapCoords::MapCoordsIterator &a,
                  const MapCoords::MapCoordsIterator &b) {
    return a.q == b.q && a.r == b.r;
  }

  bool operator!=(const MapCoords::MapCoordsIterator &a,
                  const MapCoords::MapCoordsIterator &b) {
    return a.q != b.q || a.r != b.r;
  }

  Map::Map(const MapBounds &map_bounds) : map_bounds(map_bounds) {
  }

  Map Map::build_map_of_size(size_t map_size) {
    MapBounds map_bounds = MapBounds::from_radius(map_size);
    Map map{map_bounds};
    for (const auto &coord: MapCoords(map_size)) {
      Hex *hex = new Hex();
      map.hexes.insert(std::make_pair(coord, hex));

      for (const auto &corner_direction: HEX_CORNER_DIRECTIONS) {
        CornerCoord raw_corner_coord = CornerCoord{coord, corner_direction};
        CornerCoord normalized_corner_coord =
            map.get_normalized_corner_coord(raw_corner_coord);
        Corner *corner;
        if (auto map_find_result = map.corners.find(raw_corner_coord);
          map_find_result != map.corners.end()) {
          corner = map_find_result->second;
        } else {
          corner = new Corner();
          map.corners.insert(std::make_pair(normalized_corner_coord, corner));
        }
        hex->corners.insert(std::make_pair(corner_direction, corner));
        corner->hexes.insert(
          std::make_pair(opposite_direction(corner_direction), hex));
      }

      for (const auto &edge_direction: HEX_EDGE_DIRECTIONS) {
        EdgeCoord raw_edge_coord = EdgeCoord{coord, edge_direction};
        EdgeCoord normalized_edge_coord =
            map.get_normalized_edge_coord(raw_edge_coord);
        Edge *edge;
        if (auto map_find_result = map.edges.find(normalized_edge_coord);
          map_find_result != map.edges.end()) {
          edge = map_find_result->second;
        } else {
          edge = new Edge();
          map.edges.insert(std::make_pair(normalized_edge_coord, edge));
          // map to neigbouring corners:
          for (const auto &edge_to_corner_direction:
               EDGE_TO_CORNER_DIRECTION_MAPPING[static_cast<int>(
                 edge_direction)]) {
            const auto corner_direction = edge_direction_to_corner_direction(
              edge_direction, edge_to_corner_direction);
            const CornerCoord &normalized_corner_coord =
                map.get_normalized_corner_coord({coord, corner_direction});
            auto corner_ptr = map.corners.find(normalized_corner_coord)->second;
            corner_ptr->edges.insert(std::make_pair(
              opposite_direction(edge_to_corner_direction), edge));
            edge->corners.insert(
              std::make_pair(edge_to_corner_direction, corner_ptr));
          }
        }
        hex->edges.insert(std::make_pair(edge_direction, edge));
        edge->hexes.insert(
          std::make_pair(opposite_direction(edge_direction), hex));
      }
    }

    return map;
  }

  const std::unordered_map<HexCoord2, Hex *> Map::get_hexes() const {
    return hexes;
  }
} // namespace Map
