//
// Created by Ion Plamadeala on 05/12/2025.
//
#include "edge_actor.hh"

#include "raymath.h"

namespace GameActors {
    EdgeActor::EdgeActor(Map::Edge *edge, const Map::EdgeCoord &nominal_edge_coord,
                         const Vector2 &position) : edge(edge),
                                                    nominal_edge_coord(nominal_edge_coord), position(position),
                                                    is_highlighted(false) {
    }

    void EdgeActor::update(const float delta_time) {
        animations.tick_animations(delta_time);
    }

    void EdgeActor::set_highlighted(const bool highlighted) {
        if (highlighted) {
            if (!is_highlighted) {
                const auto linear_animation = new LinearAnimation(0.2f, .0f, 1.0f, OnAnimationFinished::DO_NOTHING);
                animations.add_animation_for(EdgeActorAnimations::HIGHLIGHTED, linear_animation);
            } else {
                animations.get_animation_for(EdgeActorAnimations::HIGHLIGHTED)->reset();
            }
        } else {
            if (is_highlighted) {
                animations.remove_animation_for(EdgeActorAnimations::HIGHLIGHTED);
            }
        }
        is_highlighted = highlighted;
    }


    bool EdgeActor::get_highlighted() const {
        return is_highlighted && animations.get_animation_for(EdgeActorAnimations::HIGHLIGHTED)->is_finished();
    }

    void EdgeActor::render() const {
        const auto &engine_settings = get_engine_settings();
        const auto &color_scheme = engine_settings.get_color_scheme();
        const auto &render_settings = engine_settings.get_render_settings();
        const auto dir = static_cast<float>(nominal_edge_coord.edge_direction);
        const float move_dir = PI / 3.0f * (dir + 2.0);
        const float width = render_settings.full_hex_size - 1.80f * render_settings.hex_size * 1.0f / 3.0f;
        const float starting_delta = render_settings.hex_size * 0.90f / 3.0f;
        Rectangle rectangle{
            .x = position.x + cos(move_dir) * starting_delta,
            .y = position.y + sin(move_dir) * starting_delta,
            .width = width,
            .height = 19.0f
        };
        if (edge->road != nullptr) {
            if (const auto animation = animations.get_potential_animation_for(EdgeActorAnimations::ON_BUILD);
                animation == nullptr) {
                DrawRectanglePro(rectangle, {0.0f, 9.5f},
                                 60.0f * (static_cast<float>(nominal_edge_coord.edge_direction) + 2.0f),
                                 GREEN);
            } else {
                DrawRectanglePro(rectangle, {0.0f, 9.5f},
                                 60.0f * (static_cast<float>(nominal_edge_coord.edge_direction) + 2.0f),
                                 color_scheme.mapBorder);
                rectangle.y -= width * animation->get_current_value();
                DrawRectanglePro(rectangle, {0.0f, 9.5f},
                                 60.0f * (static_cast<float>(nominal_edge_coord.edge_direction) + 2.0f),
                                 GREEN);
            }
        } else {
            DrawRectanglePro(rectangle, {0.0f, 9.5f},
                             60.0f * (static_cast<float>(nominal_edge_coord.edge_direction) + 2.0f),
                             color_scheme.mapBorder);
        }
        if (is_highlighted) {
            const float radius = render_settings.hex_size * 0.7f / 3.0f * animations.get_animation_for(
                                     EdgeActorAnimations::HIGHLIGHTED)->get_current_value();
            DrawCircleLinesV(
                {
                    .x = position.x + cos(move_dir) * (width / 2 + starting_delta),
                    .y = position.y + sin(move_dir) * (width / 2 + starting_delta),
                }, radius, WHITE);
        }
    }

    const Map::EdgeCoord &EdgeActor::get_edge_coord() const {
        return nominal_edge_coord;
    }

    const Map::Edge *EdgeActor::get_edge() const {
        return edge;
    }

    void EdgeActor::set_road(Road *road) {
        edge->road = road;
        const auto linear_animation = new LinearAnimation(0.2f, 1.0f, 0.0f);
        animations.add_animation_for(EdgeActorAnimations::ON_BUILD, linear_animation);
    }

    EdgeActor::~EdgeActor() {
        if (is_highlighted) {
            animations.remove_animation_for(EdgeActorAnimations::HIGHLIGHTED);
        }
    }

    bool EdgeActor::is_mouse_over(const Vector2 &mouse_position) {
        const auto &render_settings = get_engine_settings().get_render_settings();
        const float dir = static_cast<float>(nominal_edge_coord.edge_direction);
        const float move_dir = (PI / 3.0f) * (dir + 2.0);
        const float width = render_settings.full_hex_size - 1.80f * render_settings.hex_size * 1.0f / 3.0f;
        const float starting_delta = render_settings.hex_size * 0.90f / 3.0f;
        return Vector2Distance(
                   {
                       .x = position.x + cos(move_dir) * (width / 2 + starting_delta),
                       .y = position.y + sin(move_dir) * (width / 2 + starting_delta),
                   }, mouse_position) < render_settings.hex_size * 0.7f / 3.0f;
    }
}
