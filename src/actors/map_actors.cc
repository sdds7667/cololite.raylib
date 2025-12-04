//
// Created by Ion Plamadeala on 13/11/2025.
//

#include "map_actors.hh"

#include "raymath.h"

namespace Engine {
    CornerActor::CornerActor(
        const RenderSettings &render_settings,
        const RenderResources &render_resources,
        const ColorScheme &color_scheme,
        Map::Corner *corner, const Map::CornerCoord &nominal_corner_coord, const Vector2 &position)
        : render_settings(render_settings), render_resources(render_resources), color_scheme(color_scheme),
          corner(corner),
          nominal_corner_coord(nominal_corner_coord), position(position), is_highlighted(false),
          base_color(color_scheme.mapBorder) {
    }

    void CornerActor::update(float delta_time) {
        animations.tick_animations(delta_time);
    }

    void CornerActor::set_highlighted(const bool highlighted) {
        if (highlighted) {
            if (!is_highlighted) {
                auto linear_animation = new LinearAnimation(0.2f, .0f, 1.0f, OnAnimationFinished::DO_NOTHING);
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


    bool CornerActor::get_highlighted() const {
        return is_highlighted && animations.get_animation_for(CornerActorAnimations::HIGHLIGHTED)->is_finished();
    }

    void CornerActor::render() const {
        Color color = base_color;
        auto outline_color = WHITE;
        if (base_color.g == 228) {
            outline_color = color_scheme.mapBorder;
        }
        auto circle_size = render_settings.hex_size * 1.0f / 3.0f;
        DrawCircleV(position, circle_size, color);
        if (is_highlighted) {
            BaseAnimation *animation = animations.get_animation_for(CornerActorAnimations::HIGHLIGHTED);
            DrawCircleLinesV(
                position,
                render_settings.hex_size * 1.0f / 3.0f * animation->get_current_value(),
                outline_color);
        }
        if (corner->house != nullptr) {
            auto texture = render_resources.sprites.house;
            if (corner->house->level == 2) {
                texture = render_resources.sprites.town;
            }
            float scale = render_settings.hex_size * (1.5f / 3.0f) / static_cast<float>(texture.width);

            float y_pos = position.y - scale * static_cast<float>(texture.height) / 2.0f;
            auto *animation = animations.get_potential_animation_for(CornerActorAnimations::BUILD_HOUSE);
            if (animation != nullptr) {
                y_pos -= animation->get_current_value() * render_settings.hex_size * 1.0f / 3.0f;
            }

            const Vector2 sprite_position = {
                .x = position.x - scale * static_cast<float>(texture.width) / 2.0f,
                .y = y_pos,
            };

            DrawTextureEx(texture, sprite_position, 0.0f, scale,
                          WHITE);
            if (is_upgradable) {
                auto &upgrade_texture = render_resources.ui.upgrade;
                auto upgrade_scale = scale * .75f;
                Vector2 upgrade_sprite_center_angle = {cos(-PI / 3), sin(-PI / 3)};
                Vector2 sprite_center_position = Vector2Scale(upgrade_sprite_center_angle, circle_size * .7);
                Vector2 sprite_size = {
                    static_cast<float>(upgrade_texture.width) * upgrade_scale,
                    static_cast<float>(upgrade_texture.height) * upgrade_scale
                };
                Vector2 upgrade_final_position =
                        Vector2Add(position,
                                   Vector2Subtract(sprite_center_position, Vector2Scale(sprite_size, .5f)));

                DrawTextureEx(upgrade_texture, upgrade_final_position, 0.0f, upgrade_scale, WHITE);
            }
        }
    }

    const Map::CornerCoord &CornerActor::get_corner_coord() const {
        return nominal_corner_coord;
    }

    const Map::Corner *CornerActor::get_corner() const {
        return corner;
    }

    void CornerActor::set_house(House *house) {
        corner->house = house;
        LinearAnimation *linear_animation = new LinearAnimation(0.2f, 1.0f, 0.0f);
        animations.add_animation_for(CornerActorAnimations::BUILD_HOUSE, linear_animation);
    }

    void CornerActor::set_upgradable(bool upgradable) {
        is_upgradable = upgradable;
    }

    bool CornerActor::get_is_upgradable() const {
        return is_upgradable;
    }

    void CornerActor::set_base_color(Color base_color) {
        this->base_color = base_color;
    }

    CornerActor::~CornerActor() {
        if (is_highlighted) {
            animations.remove_animation_for(CornerActorAnimations::HIGHLIGHTED);
        }
        animations.remove_animation_for(CornerActorAnimations::BUILD_HOUSE);
    }

    bool CornerActor::is_mouse_over(const Vector2 &mouse_position) {
        return Vector2Distance(position, mouse_position) < render_settings.hex_size * 1.0f / 3.0f;
    }

    EdgeActor::EdgeActor(
        const RenderSettings &render_settings,
        const RenderResources &render_resources,
        const ColorScheme &color_scheme,
        Map::Edge *edge, const Map::EdgeCoord &nominal_edge_coord, const Vector2 &position)
        : render_settings(render_settings), render_resources(render_resources), color_scheme(color_scheme),
          edge(edge),
          nominal_edge_coord(nominal_edge_coord), position(position), is_highlighted(false) {
    }

    void EdgeActor::update(float delta_time) {
        animations.tick_animations(delta_time);
    }

    void EdgeActor::set_highlighted(const bool highlighted) {
        if (highlighted) {
            if (!is_highlighted) {
                auto linear_animation = new LinearAnimation(0.2f, .0f, 1.0f, OnAnimationFinished::DO_NOTHING);
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
        float dir = static_cast<float>(nominal_edge_coord.edge_direction);
        float move_dir = (PI / 3.0f) * (dir + 2.0);
        float width = render_settings.full_hex_size - 1.80f * render_settings.hex_size * 1.0f / 3.0f;
        float starting_delta = render_settings.hex_size * 0.90f / 3.0f;
        Rectangle rectangle{
            .x = position.x + cos(move_dir) * starting_delta,
            .y = position.y + sin(move_dir) * starting_delta,
            .width = width,
            .height = 19.0f
        };
        if (edge->road != nullptr) {
            auto animation = animations.get_potential_animation_for(EdgeActorAnimations::ON_BUILD);
            if (animation == nullptr) {
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
            float radius = render_settings.hex_size * 0.7f / 3.0f * animations.get_animation_for(
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
        auto linear_animation = new LinearAnimation(0.2f, 1.0f, 0.0f);
        animations.add_animation_for(EdgeActorAnimations::ON_BUILD, linear_animation);
    }

    EdgeActor::~EdgeActor() {
        if (is_highlighted) {
            animations.remove_animation_for(EdgeActorAnimations::HIGHLIGHTED);
        }
    }

    bool EdgeActor::is_mouse_over(const Vector2 &mouse_position) {
        float dir = static_cast<float>(nominal_edge_coord.edge_direction);
        float move_dir = (PI / 3.0f) * (dir + 2.0);
        float width = render_settings.full_hex_size - 1.80f * render_settings.hex_size * 1.0f / 3.0f;
        float starting_delta = render_settings.hex_size * 0.90f / 3.0f;
        return Vector2Distance(
                   {
                       .x = position.x + cos(move_dir) * (width / 2 + starting_delta),
                       .y = position.y + sin(move_dir) * (width / 2 + starting_delta),
                   }, mouse_position) < render_settings.hex_size * 0.7f / 3.0f;
    }
}
