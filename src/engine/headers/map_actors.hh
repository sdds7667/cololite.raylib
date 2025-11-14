//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_MAP_ACTORS_HH
#define COLOLITE_MAP_ACTORS_HH

#include "actor.hh"
#include "animations.hh"
#include "map.hh"
#include "raylib.h"
#include "engine_settings.hh"

namespace Engine {
    class CornerActor : public Actor {
        const RenderSettings &render_settings;
        const ColorScheme &color_scheme;
        Map::Corner *corner;
        Map::CornerCoord nominal_corner_coord;
        Vector2 position;

        bool is_highlighted;

    protected:
        enum class CornerActorAnimations {
            HIGHLIGHTED,
        };

        AnimationContainer<CornerActorAnimations> animations{};

    public:
        CornerActor(
            const RenderSettings &render_settings,
            const ColorScheme &color_scheme,
            Map::Corner *corner, const Map::CornerCoord &nominal_corner_coord, const Vector2 &position);

        void update(float delta_time) override;

        void set_highlighted(bool highlighted);

        [[nodiscard]] bool get_highlighted() const;

        void render() const override;

        ~CornerActor() override;
    };
}

#endif //COLOLITE_MAP_ACTORS_HH
