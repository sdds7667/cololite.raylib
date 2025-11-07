
#pragma once

namespace Map {

struct HexCoord2 {
  int q;
  int r;
};

struct HexCoord3 : public HexCoord2 {
  int s;
};

HexCoord2 hex_coord_2_add(const HexCoord2 &x, const HexCoord2 y);

} // namespace Map
