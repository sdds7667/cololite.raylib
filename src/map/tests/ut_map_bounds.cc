
#include "map.hh"
#include "gtest/gtest.h"

class MapBounds_Requirement_distance_from_center : public ::testing::Test {
  /**
   * MapBounds Requirement 1:
   * A hex map of radius n allows only the hexes that are n-away from center.
   *
   * Defined test cases:
   * 001 - Map bounds of size 1, only the hex center allowed.
   * 002 - Map bounds of size 2, only the hex center and the its direct
   * neighbours are allowed.
   * */
};

TEST_F(MapBounds_Requirement_distance_from_center, MapBounds_001) {
  // 001 - Map bounds of size 1, only the hex center allowed
  // Arrange: No requirements

  // Act: Create new bounds for a map of size 1
  Map::MapBounds map_bounds = Map::MapBounds::from_radius(1);

  // Assert:
  // Outside the center radius - not allowed
  EXPECT_FALSE(map_bounds.is_within_bounds({.q = +1, .r = +0}));
  EXPECT_FALSE(map_bounds.is_within_bounds({.q = -1, .r = +0}));
  EXPECT_FALSE(map_bounds.is_within_bounds({.q = +0, .r = +1}));
  EXPECT_FALSE(map_bounds.is_within_bounds({.q = +0, .r = -1}));
  EXPECT_FALSE(map_bounds.is_within_bounds({.q = +1, .r = -1}));

  // Center Hex is fine
  EXPECT_TRUE(map_bounds.is_within_bounds({.q = +0, .r = +0}));
}
