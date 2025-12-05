#pragma once
#include "animations.hh"
#include "raylib.h"
#include "engine_settings.hh"

using namespace Engine;

namespace GameActors {
    class EdgeActor final : public IActor, public IClickableActor {
        Map::Edge *edge;
        Map::EdgeCoord nominal_edge_coord;
        Vector2 position;

        bool is_highlighted;

    protected:
        enum class EdgeActorAnimations {
            HIGHLIGHTED,
            ON_BUILD,
        };

        AnimationContainer<EdgeActorAnimations> animations{};

    public:
        EdgeActor(Map::Edge *edge, const Map::EdgeCoord &nominal_edge_coord, const Vector2 &position);

        void update(float delta_time) override;

        void set_highlighted(bool highlighted);

        [[nodiscard]] bool get_highlighted() const;

        void render() const override;

        [[nodiscard]] const Map::EdgeCoord &get_edge_coord() const;

        [[nodiscard]] const Map::Edge *get_edge() const;

        void set_road(Road *road);


        ~EdgeActor() override;

        [[nodiscard]] bool is_mouse_over(const Vector2 &mouse_position) override;
    };
}
