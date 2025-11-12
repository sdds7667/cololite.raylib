#include "coords.hh"
#include <gtest/gtest.h>
#include <ostream>

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
  auto result = hex_coord_a + hex_coord_b;

  // Assert
  EXPECT_EQ(result.q, 19);
  EXPECT_EQ(result.r, 23);
}

// Get neighbouring hex_coord
struct GetNeighbouringHexCoordTestParam {
  Map::HexCoord2 source;
  Map::HexEdgeDirection direction;
  Map::HexCoord2 expected;
};

std::ostream &operator<<(std::ostream &os,
                         const GetNeighbouringHexCoordTestParam &param) {
  return os << "Source: " << param.source << ", Direction: " << param.direction
            << ", Expected: " << param.expected;
}

class GetNeighbouringHexCoordTest
    : public testing::Test,
      public testing::WithParamInterface<GetNeighbouringHexCoordTestParam> {};

TEST_P(GetNeighbouringHexCoordTest, Basic_Coord_Test) {
  // Arrange
  auto param = GetParam();
  // Act
  auto result = param.source.get_neighbouring_hex_coord(param.direction);
  // Assert
  EXPECT_EQ(result, param.expected);
}

constexpr GetNeighbouringHexCoordTestParam params[] = {
    {.source = {.q = 0, .r = 0},
     .direction = Map::HexEdgeDirection::TOP,
     .expected = {.q = 0, .r = -1}},
    {.source = {.q = 0, .r = 0},
     .direction = Map::HexEdgeDirection::TOP_RIGHT,
     .expected = {.q = 1, .r = -1}},
    {.source = {.q = 0, .r = 0},
     .direction = Map::HexEdgeDirection::TOP_LEFT,
     .expected = {.q = -1, .r = 0}},
    {.source = {.q = 0, .r = 0},
     .direction = Map::HexEdgeDirection::BOTTOM,
     .expected = {.q = 0, .r = +1}},
    {.source = {.q = 0, .r = 0},
     .direction = Map::HexEdgeDirection::BOTTOM_RIGHT,
     .expected = {.q = +1, .r = 0}},
    {.source = {.q = 0, .r = 0},
     .direction = Map::HexEdgeDirection::BOTTOM_LEFT,
     .expected = {.q = -1, .r = +1}},
};

INSTANTIATE_TEST_SUITE_P(ZeroCenteredTests, GetNeighbouringHexCoordTest,
                         testing::ValuesIn(params));
