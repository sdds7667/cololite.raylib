

#pragma once

#include "raylib.h"
#include "actor.hh"

using namespace Engine;

namespace GameActors {
    class HexActor final : public IActor {
    public:
        void update(float deltaTime) override;

        void render() const override;
    };
}
