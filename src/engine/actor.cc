//
// Created by Ion Plamadeala on 13/11/2025.
//

#include "actor.hh"

#include "engine_settings.hh"
#include "raylib.h"
#include "raymath.h"

namespace Engine {
    FixedSizedTextActor::FixedSizedTextActor(const std::string &text, const Font &font, Color color, float font_size,
                                             Vector2 position) : m_font(font), m_color(color), m_font_size(font_size) {
        set_text(text);
        BoundedBoxActor::set_position(position);
    }

    void FixedSizedTextActor::update(float deltaTime) {
    }

    void FixedSizedTextActor::render() const {
        DrawTextEx(m_font, m_text.c_str(), {m_bounding_box.x, m_bounding_box.y}, m_font_size, 0.0f, m_color);
    }

    void FixedSizedTextActor::set_text(const std::string &text) {
        m_text = text;
        auto result = MeasureTextEx(m_font, text.c_str(), m_font_size, 00);
        m_bounding_box.width = result.x;
        m_bounding_box.height = result.y;
        m_bounding_box.x = m_position.x - m_anchor.x * m_bounding_box.width;
        m_bounding_box.y = m_position.y - m_anchor.y * m_bounding_box.height;
    }

    void FixedSizedTextActor::set_color(Color color) { m_color = color; }


    void ContainerActor::update_children_positions() const {
        for (int i = 0; i < m_actors.size(); i++) {
            auto *actor = m_actors[i];
            actor->set_position({
                m_bounding_box.x + m_bounding_box.width * m_actor_relative_positions[i].x,
                m_bounding_box.y + m_bounding_box.height * m_actor_relative_positions[i].y
            });
        }
    }

    void ContainerActor::recalculate_bounding_box() {
        m_bounding_box.x = m_position.x - m_anchor.x * m_bounding_box.width;
        m_bounding_box.y = m_position.y - m_anchor.y * m_bounding_box.height;
        if (changes_propagated_to_children) {
            update_children_positions();
        }
    }

    void ContainerActor::update_relative_positions() {
        for (int i = 0; i < m_actors.size(); i++) {
            m_actor_relative_positions[i] = {
                (m_actors[i]->get_position().x - m_bounding_box.x) / m_bounding_box.width,
                (m_actors[i]->get_position().y - m_bounding_box.y) /
                m_bounding_box.height
            };
        }
    }

    ContainerActor::ContainerActor(const Vector2 &position, const std::vector<BoundedBoxActor *> &actors) {
        m_position = position;
        changes_propagated_to_children = true;
        // bool bounding_box_initialized = false;
        for (const auto actor: actors) {
            m_actors.push_back(actor);
            m_actor_relative_positions.push_back({0.0f, 0.0f});
            auto anchored_actor_position = actor->get_anchored_position();
            m_bounding_box.x = std::min(m_bounding_box.x, anchored_actor_position.x);
            m_bounding_box.y = std::min(m_bounding_box.y, anchored_actor_position.y);

            const auto &[actor_width, actor_height] = actor->get_size();
            m_bounding_box.width =
                    std::max(m_bounding_box.width, anchored_actor_position.x - m_bounding_box.x + actor_width);
            m_bounding_box.height =
                    std::max(m_bounding_box.height, anchored_actor_position.y - m_bounding_box.y + actor_height);
        }
        update_relative_positions();
        recalculate_bounding_box();
    }


    void ContainerActor::add_actor_at_relative_position(BoundedBoxActor *actor, const Vector2 &relative_position) {
        m_actors.push_back(actor);
        m_actor_relative_positions.push_back(relative_position);
        auto anchored_actor_position = actor->get_anchored_position();
        m_bounding_box.x = std::min(m_bounding_box.x, anchored_actor_position.x);
        m_bounding_box.y = std::min(m_bounding_box.y, anchored_actor_position.y);
        const auto &[actor_width, actor_height] = actor->get_size();
        m_bounding_box.width = std::max(m_bounding_box.width, anchored_actor_position.x + actor_width);
        m_bounding_box.height = std::max(m_bounding_box.height, anchored_actor_position.y + actor_height);
        update_relative_positions();
    }

    void ContainerActor::set_position(const Vector2 &position) {
        BoundedBoxActor::set_position(position);
        recalculate_bounding_box();
    }

    void ContainerActor::remove_actor(const BoundedBoxActor *actor) { throw std::runtime_error("Not implemented"); }

    void ContainerActor::set_background_color(const std::optional<Color> &color) {
        m_background_color = color;
    }


    void ContainerActor::update(float deltaTime) {
        for (const auto &actor: m_actors) {
            actor->update(deltaTime);
        }
    }

    void ContainerActor::render() const {
        if (m_background_color.has_value()) {
            DrawRectangleRounded(m_bounding_box, .1, 5, m_background_color.value());
        }
        for (const auto &actor: m_actors) {
            actor->render();
        }
        // DrawCircleV(get_position(), 5.f, RED);
    }

    void ContainerActor::cleanup() const {
        for (const auto actor: m_actors) {
            delete actor;
        }
    }

    SpriteActor::SpriteActor(Vector2 position) {
        BoundedBoxActor::set_position(position);
    }

    SpriteActor::SpriteActor(const Texture2D &texture, Vector2 position, AlignmentAnchor anchor) : texture(texture) {
        SpriteActor::set_anchor(anchor);
        BoundedBoxActor::set_bounding_box({
            0.f, 0.f, static_cast<float>(texture.width), static_cast<float>(texture.height)
        });
        BoundedBoxActor::set_position(position);
    }

    void SpriteActor::set_position(const Vector2 &position) {
        m_position = position;
        m_bounding_box.x = m_position.x - m_anchor.x * m_bounding_box.width * m_scale;
        m_bounding_box.y = m_position.y - m_anchor.y * m_bounding_box.height * m_scale;
    }

    ResourceDisplayActor::ResourceDisplayActor(const RenderResources &render_resources,
                                               const std::unordered_map<Map::Resource, int> &
                                               player_resources) : ContainerActor(Vector2Zero(), {}) {
        std::vector<BoundedBoxActor *> resource_containers;
        float starting_x = 0.0f;
        float starting_y = 0.0f;
        float texture_width = 64.0f;
        for (const auto resource: {
                 Map::Resource::WOOD, Map::Resource::BRICK, Map::Resource::SHEEP,
                 Map::Resource::WHEAT, Map::Resource::STONE
             }) {
            auto &outline_texture = render_resources.resource_sprites.resource_outline;
            const auto &texture = get_texture_for_resource(render_resources, resource);
            float scale = texture_width / static_cast<float>(texture.width);

            auto *sprite_actor = new SpriteActor(texture, Vector2Zero(), AlignmentAnchor::MIDDLE_CENTER);
            sprite_actor->set_scale(scale);
            auto *outline_actor = new SpriteActor(outline_texture, Vector2Zero(), AlignmentAnchor::MIDDLE_CENTER);
            outline_actor->set_scale(scale * 1.5f);
            auto container_actor = new ContainerActor({starting_x, starting_y}, {outline_actor, sprite_actor});
            resource_containers.emplace_back(container_actor);
            resource_sprites[resource] = container_actor;

            auto *fixed_sized_text_actor =
                    new FixedSizedTextActor("100", render_resources.map_font, WHITE, 32.0f,
                                            Vector2{
                                                starting_x + outline_texture.width * scale * 1.5f + 30,
                                                starting_y + outline_texture.height * scale * 1.5f / 2.0f
                                            });
            resource_text_actors[resource] = fixed_sized_text_actor;
            fixed_sized_text_actor->set_anchor(AlignmentAnchor::MIDDLE_CENTER);

            auto *resource_delta =
                    new FixedSizedTextActor("+100", render_resources.map_font, WHITE, 32.0f,
                                            Vector2{
                                                starting_x + outline_texture.width * scale * 1.5f + 30 * 2 +
                                                fixed_sized_text_actor->get_size().x,
                                                starting_y + outline_texture.height * scale * 1.5f / 2.0f
                                            });
            delta_resources_actors[resource] = resource_delta;
            resource_delta->set_anchor(AlignmentAnchor::MIDDLE_CENTER);
            auto bounding_box_actor = new BoundedBoxActor{};
            Rectangle bounding_box = {
                .x = 0.0f,
                .y = 0.0f,
                .width = resource_delta->get_anchored_position().x + resource_delta->get_size().x - starting_x,
                .height = outline_texture.height * scale * 1.5f
            };
            bounding_box_actor->set_bounding_box(bounding_box);
            bounding_box_actor->set_position(Vector2{starting_x, starting_y});
            resource_bounding_box[resource] = bounding_box_actor;
            starting_y += outline_texture.height * scale * 1.5f + 10.f;
            resource_containers.emplace_back(fixed_sized_text_actor);
            resource_containers.emplace_back(resource_delta);
            resource_containers.emplace_back(bounding_box_actor);
        }

        changes_propagated_to_children = true;
        // bool bounding_box_initialized = false;
        for (const auto actor: resource_containers) {
            m_actors.push_back(actor);
            m_actor_relative_positions.push_back({0.0f, 0.0f});
            auto anchored_actor_position = actor->get_anchored_position();
            m_bounding_box.x = std::min(m_bounding_box.x, anchored_actor_position.x);
            m_bounding_box.y = std::min(m_bounding_box.y, anchored_actor_position.y);
            const auto &[actor_width, actor_height] = actor->get_size();
            m_bounding_box.width =
                    std::max(m_bounding_box.width, anchored_actor_position.x - m_bounding_box.x + actor_width);
            m_bounding_box.height =
                    std::max(m_bounding_box.height, anchored_actor_position.y - m_bounding_box.y + actor_height);
        }
        update_relative_positions();
        recalculate_bounding_box();
    }

    void ResourceDisplayActor::update_resources(const std::unordered_map<Map::Resource, int> &player_resources,
                                                const std::unordered_map<Map::Resource, int> &delta_resources) const {
        for (const auto [resource, amount]: player_resources) {
            resource_text_actors.at(resource)->set_text(std::to_string(amount));
        }
        for (const auto [resource, amount]: delta_resources) {
            auto resource_actor = delta_resources_actors.at(resource);
            if (amount == 0) {
                resource_actor->set_text("");
            } else if (amount > 0) {
                resource_actor->set_text("+" + std::to_string(amount));
                resource_actor->set_color(GREEN);
            } else {
                resource_actor->set_text(std::to_string(amount));
                resource_actor->set_color(RED);
            }
        }
    }

    void ResourceDisplayActor::render() const {
        ContainerActor::render();
        // for (const auto [resource, actor]: resource_bounding_box) {
        //     DrawRectangleLinesEx(actor->get_bounding_box(), 3.0f, WHITE);
        // }
    }

    std::optional<Map::Resource> ResourceDisplayActor::is_over_resource(const Vector2 &mouse_position) const {
        if (mouse_position.x < m_bounding_box.x || mouse_position.x > m_bounding_box.x + m_bounding_box.width ||
            mouse_position.y < m_bounding_box.y || mouse_position.y > m_bounding_box.y + m_bounding_box.height) {
            return std::nullopt;
        }
        for (const auto [resource, actor]: resource_bounding_box) {
            if (CheckCollisionPointRec(mouse_position, actor->get_bounding_box())) {
                return resource;
            }
        }
        return std::nullopt;
    }

    std::optional<Map::Resource> ResourceDisplayActor::is_over_resource_sprite(const Vector2 &mouse_position) const {
        if (mouse_position.x < m_bounding_box.x || mouse_position.x > m_bounding_box.x + m_bounding_box.width ||
            mouse_position.y < m_bounding_box.y || mouse_position.y > m_bounding_box.y + m_bounding_box.height) {
            return std::nullopt;
        }
        for (const auto [resource, actor]: resource_sprites) {
            if (CheckCollisionPointRec(mouse_position, actor->get_bounding_box())) {
                return resource;
            }
        }
        return std::nullopt;
    }

    ContainerActor *ResourceDisplayActor::get_drag_and_drop_resource(const RenderResources &resources,
                                                                     const Vector2 &mouse_position) const {
        for (const auto [resource, actor]: resource_sprites) {
            if (CheckCollisionPointRec(mouse_position, actor->get_bounding_box())) {
                Vector2 anchored_actor_position = actor->get_anchored_position();
                Vector2 delta = Vector2Subtract(mouse_position, anchored_actor_position);
                const auto &[actor_width, actor_height] = actor->get_size();
                Vector2 anchor = Vector2Divide(delta, {actor_width, actor_height});

                constexpr float texture_width = 64.0f;

                auto outline_texture = resources.resource_sprites.resource_outline;
                auto resource_texture = get_texture_for_resource(resources, resource);
                float scale = (texture_width) / resource_texture.width;
                auto *outline_actor = new SpriteActor(outline_texture, Vector2Zero(), AlignmentAnchor::MIDDLE_CENTER);
                outline_actor->set_scale(scale * 1.5f);
                auto *resource_actor = new SpriteActor(resource_texture, Vector2Zero(), AlignmentAnchor::MIDDLE_CENTER);
                resource_actor->set_scale(scale);
                auto *drag_and_drop_actor = new ContainerActor(mouse_position, {outline_actor, resource_actor});
                drag_and_drop_actor->set_anchor(anchor);
                return drag_and_drop_actor;
            }
        }
        return nullptr;
    }


    void SpriteActor::set_scale(float scale) {
        this->m_scale = scale;
        BoundedBoxActor::set_bounding_box({
            m_position.x - m_anchor.x * static_cast<float>(texture.width) * m_scale,
            m_position.y - m_anchor.y * static_cast<float>(texture.height) * m_scale,
            texture.width * m_scale, texture.height * m_scale
        });
    }

    float SpriteActor::get_scale() const { return m_scale; }

    void SpriteActor::update(float deltaTime) {
    }

    void SpriteActor::render() const {
        const Vector2 render_position = {
            m_position.x - m_anchor.x * static_cast<float>(texture.width) * m_scale,
            m_position.y - m_anchor.y * static_cast<float>(texture.height) * m_scale

        };
        DrawTextureEx(texture, render_position, 0.0f, m_scale, WHITE);
    }
} // namespace Engine
