#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace Map {

    enum class HexEdgeDirection {
        BOTTOM_RIGHT = 0,
        BOTTOM,
        BOTTOM_LEFT,
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
    };

    const std::vector HEX_EDGE_DIRECTIONS{
            HexEdgeDirection::BOTTOM_RIGHT, HexEdgeDirection::BOTTOM, HexEdgeDirection::BOTTOM_LEFT,
            HexEdgeDirection::TOP_LEFT,     HexEdgeDirection::TOP,    HexEdgeDirection::TOP_RIGHT,
    };

    HexEdgeDirection opposite_direction(const HexEdgeDirection &hex_edge);

    enum class CornerEdgeDirection { RIGHT = 0, BOTTOM_RIGHT, BOTTOM_LEFT, LEFT, TOP_LEFT, TOP_RIGHT };

    // Note: Can be optimized. The RIGHT, BOTTOM_LEFT AND TOP_LEFT behave in the
    // same way Same as BOTTOM_RIGHT, LEFT and TOP_RIGHT
    const std::vector<std::vector<CornerEdgeDirection>> CORNER_TO_EDGE_DIRECTION_MAPPING{
            // HexCornerDirection::RIGHT
            {
                    CornerEdgeDirection::RIGHT,
                    CornerEdgeDirection::BOTTOM_LEFT,
                    CornerEdgeDirection::TOP_LEFT,
            },
            // HexCornerDirection::BOTTOM_RIGHT
            {
                    CornerEdgeDirection::BOTTOM_RIGHT,
                    CornerEdgeDirection::LEFT,
                    CornerEdgeDirection::TOP_RIGHT,
            },
            // HexCornerDirection::BOTTOM_LEFT
            {
                    CornerEdgeDirection::RIGHT,
                    CornerEdgeDirection::BOTTOM_LEFT,
                    CornerEdgeDirection::TOP_LEFT,
            },
            // HexCornerDirection::LEFT
            {
                    CornerEdgeDirection::BOTTOM_RIGHT,
                    CornerEdgeDirection::LEFT,
                    CornerEdgeDirection::TOP_RIGHT,
            },
            // HexCornerDirection::TOP_LEFT
            {
                    CornerEdgeDirection::RIGHT,
                    CornerEdgeDirection::BOTTOM_LEFT,
                    CornerEdgeDirection::TOP_LEFT,
            },
            // HexCornerDirection::TOP_RIGHT
            {
                    CornerEdgeDirection::BOTTOM_RIGHT,
                    CornerEdgeDirection::LEFT,
                    CornerEdgeDirection::TOP_RIGHT,
            },
    };

    // Note: Can be optimized. The RIGHT, BOTTOM_LEFT AND TOP_LEFT behave in the
    // same way Same as BOTTOM_RIGHT, LEFT and TOP_RIGHT
    const std::vector<std::vector<CornerEdgeDirection>> EDGE_TO_CORNER_DIRECTION_MAPPING{
            // HexEdgeDirection::BOTTOM_RIGHT
            {
                    CornerEdgeDirection::BOTTOM_LEFT,
                    CornerEdgeDirection::TOP_RIGHT,
            },
            // HexEdgeDirection::BOTTOM
            {
                    CornerEdgeDirection::RIGHT,
                    CornerEdgeDirection::LEFT,
            },
            // HexEdgeDirection::BOTTOM_LEFT
            {
                    CornerEdgeDirection::BOTTOM_RIGHT,
                    CornerEdgeDirection::TOP_LEFT,
            },
            // HexEdgeDirection::TOP_LEFT
            {
                    CornerEdgeDirection::BOTTOM_LEFT,
                    CornerEdgeDirection::TOP_RIGHT,
            },
            // HexEdgeDirection::TOP
            {
                    CornerEdgeDirection::RIGHT,
                    CornerEdgeDirection::LEFT,
            },
            // HexEdgeDirection::TOP_RIGHT
            {
                    CornerEdgeDirection::BOTTOM_RIGHT,
                    CornerEdgeDirection::TOP_LEFT,
            },
    };

    CornerEdgeDirection opposite_direction(const CornerEdgeDirection &);

    enum class HexCornerDirection {
        RIGHT = 0,
        BOTTOM_RIGHT,
        BOTTOM_LEFT,
        LEFT,
        TOP_LEFT,
        TOP_RIGHT,
    };

    const std::vector<HexCornerDirection> HEX_CORNER_DIRECTIONS{
            HexCornerDirection::RIGHT, HexCornerDirection::BOTTOM_RIGHT, HexCornerDirection::BOTTOM_LEFT,
            HexCornerDirection::LEFT,  HexCornerDirection::TOP_LEFT,     HexCornerDirection::TOP_RIGHT,
    };

    HexCornerDirection opposite_direction(const HexCornerDirection &);
    HexCornerDirection edge_direction_to_corner_direction(const HexEdgeDirection &edge_direction,
                                                          const CornerEdgeDirection &edge_corner_direction);

    struct EdgeCoord;
    struct CornerCoord;

    struct HexCoord2 {
        int q;
        int r;

        bool operator==(const HexCoord2 &rhs) const;
        HexCoord2 operator+(const HexCoord2 &rhs) const;
        [[nodiscard]] HexCoord2 get_neighbouring_hex_coord(HexEdgeDirection neighbour_direction) const;
        [[nodiscard]] EdgeCoord get_edge_coord(HexEdgeDirection edge_direction) const;
        [[nodiscard]] CornerCoord get_corner_coord(HexCornerDirection corner_direction) const;
    };

    struct EdgeCoord {
        HexCoord2 hex_coord;
        HexEdgeDirection edge_direction;
        bool operator==(const EdgeCoord &other) const;
    };

    struct CornerCoord {

        HexCoord2 hex_coord;
        HexCornerDirection corner_direction;
        bool operator==(const CornerCoord &other) const;
    };

} // namespace Map

// HexCoord2
std::string to_string(const Map::HexCoord2 &coord);

// EdgeDirection
std::string to_string(const Map::HexEdgeDirection &edge_direction);

// CornerDirection
std::string to_string(const Map::HexCornerDirection &corner_direction);

// EdgeCoord
std::string to_string(const Map::EdgeCoord &edge_coord);

// CornerCoord
std::string to_string(const Map::CornerCoord &corner_coord);

std::string to_string(const Map::CornerEdgeDirection &corner_edge_direction);


std::ostream &operator<<(std::ostream &output_stream, const Map::HexCoord2 &coord);
std::ostream &operator<<(std::ostream &output_stream, const Map::HexCornerDirection &corner_direction);
std::ostream &operator<<(std::ostream &output_stream, const Map::HexEdgeDirection &edge_direction);
std::ostream &operator<<(std::ostream &output_stream, const Map::EdgeCoord &edge_coord);
std::ostream &operator<<(std::ostream &output_stream, const Map::CornerCoord &corner_coord);
