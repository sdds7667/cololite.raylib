//
// Created by Ion Plamadeala on 05/12/2025.
//

#include "corner_actor.hh"
#include "raymath.h"

namespace GameActors {
    CornerActor::CornerActor(Map::Corner *corner, const Map::CornerCoord &nominal_corner_coord,
                             const Vector2 &position) : m_corner(corner),
                                                        m_nominal_corner_coord(nominal_corner_coord),
                                                        m_position(position),
                                                        is_upgradable(false),
                                                        is_highlighted(false) {
        m_base_color = get_engine_settings().get_color_scheme().mapBorder;
    }

    void CornerActor::update(const float delta_time) {
        animations.tick_animations(delta_time);
    }

    void CornerActor::set_highlighted(const bool highlighted) {
        if (highlighted) {
            if (!is_highlighted) {
                const auto linear_animation = new LinearAnimation(0.2f, .0f, 1.0f, OnAnimationFinished::DO_NOTHING);
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
        const auto &engine_settings = get_engine_settings();
        const auto &color_scheme = engine_settings.get_color_scheme();
        const auto &render_settings = engine_settings.get_render_settings();
        const auto &render_resources = engine_settings.get_render_resources();

        const Color color = m_base_color;
        auto outline_color = WHITE;
        if (m_base_color.g == 228) {
            outline_color = color_scheme.mapBorder;
        }
        const auto circle_size = render_settings.hex_size * 1.0f / 3.0f;
        DrawCircleV(m_position, circle_size, color);
        if (is_highlighted) {
            const BaseAnimation *animation = animations.get_animation_for(CornerActorAnimations::HIGHLIGHTED);
            DrawCircleLinesV(
                m_position,
                render_settings.hex_size * 1.0f / 3.0f * animation->get_current_value(),
                outline_color);
        }
        if (m_corner->house != nullptr) {
            auto texture = render_resources.sprites.house;
            if (m_corner->house->level == 2) {
                texture = render_resources.sprites.town;
            }
            const float scale = render_settings.hex_size * (1.5f / 3.0f) / static_cast<float>(texture.width);

            float y_pos = m_position.y - scale * static_cast<float>(texture.height) / 2.0f;
            if (const auto *animation = animations.get_potential_animation_for(CornerActorAnimations::BUILD_HOUSE);
                animation != nullptr) {
                y_pos -= animation->get_current_value() * render_settings.hex_size * 1.0f / 3.0f;
            }

            const Vector2 sprite_position = {
                .x = m_position.x - scale * static_cast<float>(texture.width) / 2.0f,
                .y = y_pos,
            };

            DrawTextureEx(texture, sprite_position, 0.0f, scale,
                          WHITE);
            if (is_upgradable) {
                auto &upgrade_texture = render_resources.ui.upgrade;
                const auto upgrade_scale = scale * .75f;
                const Vector2 upgrade_sprite_center_angle = {cos(-PI / 3), sin(-PI / 3)};
                const Vector2 sprite_center_position = Vector2Scale(upgrade_sprite_center_angle, circle_size * .7);
                const Vector2 sprite_size = {
                    static_cast<float>(upgrade_texture.width) * upgrade_scale,
                    static_cast<float>(upgrade_texture.height) * upgrade_scale
                };
                const Vector2 upgrade_final_position =
                        Vector2Add(m_position,
                                   Vector2Subtract(sprite_center_position, Vector2Scale(sprite_size, .5f)));

                DrawTextureEx(upgrade_texture, upgrade_final_position, 0.0f, upgrade_scale, WHITE);
            }
        }
    }

    const Map::CornerCoord &CornerActor::get_corner_coord() const {
        return m_nominal_corner_coord;
    }

    const Map::Corner *CornerActor::get_corner() const {
        return m_corner;
    }

    void CornerActor::set_house(House *house) {
        m_corner->house = house;
        const auto linear_animation = new LinearAnimation(0.2f, 1.0f, 0.0f);
        animations.add_animation_for(CornerActorAnimations::BUILD_HOUSE, linear_animation);
    }

    void CornerActor::set_upgradable(const bool upgradable) {
        is_upgradable = upgradable;
    }

    bool CornerActor::get_is_upgradable() const {
        return is_upgradable;
    }

    auto CornerActor::set_base_color(const Color base_color) -> void {
        this->m_base_color = base_color;
    }

    CornerActor::~CornerActor() {
        if (is_highlighted) {
            animations.remove_animation_for(CornerActorAnimations::HIGHLIGHTED);
        }
        animations.remove_animation_for(CornerActorAnimations::BUILD_HOUSE);
    }

    bool CornerActor::is_mouse_over(const Vector2 &mouse_position) {
        const auto &render_settings = get_engine_settings().get_render_settings();
        return Vector2Distance(m_position, mouse_position) < render_settings.hex_size * 1.0f / 3.0f;
    }
}
