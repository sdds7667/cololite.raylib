
#pragma once

#include "generic_actors.hh"
#include "map.hh"

using namespace Engine;

namespace GameActors {
    class MapActor final : public PositionedActor, IClickableActor {
        LayeredContainer m_actors{};
        const Map::Map &m_map;

    public:
        explicit MapActor(const Vector2 &position, const Map::Map &map);

        ~MapActor() override = default;

        [[nodiscard]] auto get_children() -> LayeredContainer &;

        [[nodiscard]] auto get_children() const -> const LayeredContainer &;

        void update(float deltaTime) override;

        void render() const override;

        auto is_mouse_over(const Vector2 &mouse_position) -> bool override;

        void on_mouse_entered(const Vector2 &mouse_position) override;

        void on_mouse_exited(const Vector2 &mouse_position) override;

        void on_mouse_pressed(const Vector2 &mouse_position) override;

        void on_mouse_released(const Vector2 &mouse_position) override;
    };
} // namespace GameActors
