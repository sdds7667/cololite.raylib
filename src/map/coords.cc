
#include "coords.hh"

namespace Map {
HexCoord2 hex_coord_2_add(const HexCoord2 &x, const HexCoord2 y) {
  return {x.q + y.q, x.r + y.r};
}
} // namespace Map
