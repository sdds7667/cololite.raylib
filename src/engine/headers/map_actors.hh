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
    class CornerActor : public Actor, public ClickableActor {
        const RenderSettings &render_settings;
        const RenderResources &render_resources;
        const ColorScheme &color_scheme;
        Map::Corner *corner;
        Map::CornerCoord nominal_corner_coord;
        Vector2 position;

        bool is_highlighted;

    protected:
        enum class CornerActorAnimations {
            HIGHLIGHTED,
            BUILD_HOUSE,
        };

        AnimationContainer<CornerActorAnimations> animations{};

    public:
        CornerActor(
            const RenderSettings &render_settings,
            const RenderResources &render_resources,
            const ColorScheme &color_scheme,
            Map::Corner *corner, const Map::CornerCoord &nominal_corner_coord, const Vector2 &position);

        void update(float delta_time) override;

        void set_highlighted(bool highlighted);

        [[nodiscard]] bool get_highlighted() const;

        void render() const override;

        [[nodiscard]] const Map::CornerCoord &get_corner_coord() const;

        [[nodiscard]] const Map::Corner *get_corner() const;

        void set_house(House *house);

        ~CornerActor() override;

        [[nodiscard]] bool is_clicked(const Vector2 &mouse_position) const override;
    };

    class EdgeActor : public Actor, public ClickableActor {
        const RenderSettings &render_settings;
        const RenderResources &render_resources;
        const ColorScheme &color_scheme;
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
        EdgeActor(
            const RenderSettings &render_settings,
            const RenderResources &render_resources,
            const ColorScheme &color_scheme,
            Map::Edge *edge, const Map::EdgeCoord &nominal_edge_coord, const Vector2 &position);

        void update(float delta_time) override;

        void set_highlighted(bool highlighted);

        [[nodiscard]] bool get_highlighted() const;

        void render() const override;

        [[nodiscard]] const Map::EdgeCoord &get_edge_coord() const;

        [[nodiscard]] const Map::Edge *get_edge() const;

        void set_road(Road *road);


        ~EdgeActor() override;

        [[nodiscard]] bool is_clicked(const Vector2 &mouse_position) const override;
    };
}

#endif //COLOLITE_MAP_ACTORS_HH
