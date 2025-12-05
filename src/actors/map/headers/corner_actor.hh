
#pragma once
#include "animations.hh"
#include "raylib.h"
#include "engine_settings.hh"

using namespace Engine;

namespace GameActors {
    class CornerActor final : public IActor, public IClickableActor {
        Map::Corner *m_corner;
        Map::CornerCoord m_nominal_corner_coord;
        Vector2 m_position;
        Color m_base_color;

        bool is_upgradable;
        bool is_highlighted;

    protected:
        enum class CornerActorAnimations {
            HIGHLIGHTED,
            BUILD_HOUSE,
        };

        AnimationContainer<CornerActorAnimations> animations{};

    public:
        CornerActor(Map::Corner *corner, const Map::CornerCoord &nominal_corner_coord, const Vector2 &position);

        void update(float delta_time) override;

        void set_highlighted(bool highlighted);

        [[nodiscard]] bool get_highlighted() const;

        void render() const override;

        [[nodiscard]] const Map::CornerCoord &get_corner_coord() const;

        [[nodiscard]] const Map::Corner *get_corner() const;

        void set_house(House *house);

        void set_upgradable(bool upgradable);

        [[nodiscard]] bool get_is_upgradable() const;

        void set_base_color(Color base_color);

        ~CornerActor() override;

        [[nodiscard]] bool is_mouse_over(const Vector2 &mouse_position) override;
    };
}
