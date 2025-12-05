

#pragma once

#include "raylib.h"
#include "actor.hh"

using namespace Engine;

namespace GameActors {
    class HexActor final : public PositionedActor {
        float m_hex_radius;
        Map::Hex *m_hex;

    public:
        HexActor(Vector2 position, float hex_radius, Map::Hex *hex);

        void update(float deltaTime) override;

        void render() const override;
    };
}
