//
// Created by Ion Plamadeala on 13/11/2025.
//

#include "map_actors.hh"

namespace Engine {
    CornerActor::CornerActor(
        const RenderSettings &render_settings,
        const ColorScheme &color_scheme,
        Map::Corner *corner, const Map::CornerCoord &nominal_corner_coord, const Vector2 &position)
        : render_settings(render_settings), color_scheme(color_scheme), corner(corner),
          nominal_corner_coord(nominal_corner_coord), position(position), is_highlighted(false) {
    }

    void CornerActor::update(float delta_time) {
        animations.tick_animations(delta_time);
    }

    bool CornerActor::get_highlighted() const {
        return is_highlighted && animations.get_animation_for(CornerActorAnimations::HIGHLIGHTED)->is_finished();
    }


    void CornerActor::render() const {
        DrawCircleV(position, render_settings.hex_size * 1.0f / 3.0f, color_scheme.circleBorder);
        if (is_highlighted) {
            BaseAnimation *animation = animations.get_animation_for(CornerActorAnimations::HIGHLIGHTED);
            DrawCircleLinesV(
                position,
                render_settings.hex_size * 1.0f / 3.0f * animation->get_current_value(), WHITE);
        }
    }

    CornerActor::~CornerActor() {
        if (is_highlighted) {
            animations.remove_animation_for(CornerActorAnimations::HIGHLIGHTED);
        }
    }

    void CornerActor::set_highlighted(const bool highlighted) {
        if (highlighted) {
            if (!is_highlighted) {
                auto linear_animation = new LinearAnimation(0.2f, .0f, 1.0f);
                animations.add_animation_for(CornerActorAnimations::HIGHLIGHTED, linear_animation);
            } else {
                animations.get_animation_for(CornerActorAnimations::HIGHLIGHTED)->reset();
            }
        } else {
            if (is_highlighted) {
                animations.remove_animation_for(CornerActorAnimations::HIGHLIGHTED);
            }
        }
        is_highlighted = highlighted;
    }
}
