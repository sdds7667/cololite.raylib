//
// Created by Ion Plamadeala on 15/11/2025.
//

#include "map.hh"
#include <gtest/gtest.h>


class MapCreation : public ::testing::Test {
};

TEST_F(MapCreation, MapCreation_001) {
    // Arrange: Nothing to do
    // Act: create a new map
    Map::Map map = Map::Map::build_map_of_size(1);

    // Assert
    EXPECT_EQ(7, map.get_hexes().size()) << "Map size not right";
    EXPECT_EQ(24, map.get_corners().size()) << "Map corners size different then expected";
    EXPECT_EQ(30, map.get_edges().size()) << "Map edges size different then expected";

    Map::CornerCoord nominal_corner_coord{
        .hex_coord = Map::HexCoord2{.q = 0, .r = 0},
        .corner_direction = Map::HexCornerDirection::RIGHT,
    };
    auto corner_result = map.get_corners().find(nominal_corner_coord);
    EXPECT_NE(corner_result, map.get_corners().end()) << "Corner not found in map";
    EXPECT_EQ(3, corner_result->second->hexes.size()) << "Corner not connected to the right amount of hexes";
}

