
#pragma once
#include "coords.hh"
#include "coords_hash.hh"
#include <unordered_map>

namespace Map {
struct Hex;
struct Corner;
struct Edge;

struct Hex {
  std::unordered_map<HexCornerDirection, Corner *> corners;
  std::unordered_map<HexEdgeDirection, Edge *> edges;
};

struct Corner {
  std::unordered_map<HexCornerDirection, Hex *> hexes;
  std::unordered_map<CornerEdgeDirection, Edge *> edges;
};

struct Edge {
  std::unordered_map<HexEdgeDirection, Hex *> hexes;
  std::unordered_map<CornerEdgeDirection, Corner *> corners;
};

struct MapBounds {
  HexCoord2 min_bounds;
  HexCoord2 max_bounds;

  static MapBounds from_radius(size_t radius);

  bool is_within_bounds(const HexCoord2 &coord) const;
};

class Map {

private:
  const MapBounds map_bounds;
  std::unordered_map<HexCoord2, Hex *> hexes;
  std::unordered_map<CornerCoord, Corner *> corners;
  std::unordered_map<EdgeCoord, Edge *> edges;

  Map(const MapBounds &map_bounds);

public:
  CornerCoord get_normalized_corner_coord(const CornerCoord &raw_coord) const;
  EdgeCoord get_normalized_edge_coord(const EdgeCoord &edge_coord) const;
  static Map build_map_of_size(size_t map_size);
  const std::unordered_map<HexCoord2, Hex *> get_hexes() const;
};

class MapCoords {
private:
  int m_size;

public:
  MapCoords(int size);

  class MapCoordsIterator {
  public:
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = HexCoord2;
    using pointer = const HexCoord2 *; // or also value_type*
    using reference = const HexCoord2 &;

    MapCoordsIterator(int size);

    MapCoordsIterator(int size, int r, int q);

    reference operator*() const;
    pointer operator->() const;

    MapCoordsIterator &operator++();
    MapCoordsIterator operator++(int);

    friend bool operator==(const MapCoordsIterator &a,
                           const MapCoordsIterator &b);
    friend bool operator!=(const MapCoordsIterator &a,
                           const MapCoordsIterator &b);

  private:
    int size;
    int r;
    int max_r;
    int q;
    int max_q;
    HexCoord2 current_coord;
  };

  MapCoordsIterator begin();
  MapCoordsIterator end();
};

bool operator==(const MapCoords::MapCoordsIterator &a,
                const MapCoords::MapCoordsIterator &b);
bool operator!=(const MapCoords::MapCoordsIterator &a,
                const MapCoords::MapCoordsIterator &b);

} // namespace Map
