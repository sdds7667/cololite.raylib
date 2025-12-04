
#pragma once

#include "generic_actors.hh"
#include "map_actors.hh"

using namespace Engine;

namespace GameActors {
    class MapActor : public IActor, IClickableActor {

    public:
        MapActor(Map::Map &map);

        void update(float deltaTime) override;

        void render() const override;

        auto is_mouse_over(const Vector2 &mouse_position) -> bool override;

        void on_mouse_entered(const Vector2 &mouse_position) override;

        void on_mouse_exited(const Vector2 &mouse_position) override;

        void on_mouse_pressed(const Vector2 &mouse_position) override;

        void on_mouse_released(const Vector2 &mouse_position) override;
    }
} // namespace GameActors
