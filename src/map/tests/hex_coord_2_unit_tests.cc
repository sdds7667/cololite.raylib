#include "coords.hh"
#include <gtest/gtest.h>

class HexCoord2Test : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup code if needed
  }

  void TearDown() override {
    // Cleanup code if needed
  }
};

TEST_F(HexCoord2Test, BasicTest) {
  // Arrange
  Map::HexCoord2 hex_coord_a{10, 15};
  Map::HexCoord2 hex_coord_b{9, 8};

  // Act
  auto result = Map::hex_coord_2_add(hex_coord_a, hex_coord_b);

  // Assert
  EXPECT_EQ(result.q, 19);
  EXPECT_EQ(result.r, 23);
}
