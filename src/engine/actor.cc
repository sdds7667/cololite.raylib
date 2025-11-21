//
// Created by Ion Plamadeala on 13/11/2025.
//

#include "actor.hh"

#include "raymath.h"

namespace Engine {
    void ContainerActor::update_children_positions() const {
        for (int i = 0; i < m_actors.size(); i++) {
            auto *actor = m_actors[i];
            actor->set_position({
                    m_bounding_box.x + m_bounding_box.width * m_actor_relative_positions[i].x,
                    m_bounding_box.y + m_bounding_box.height * m_actor_relative_positions[i].y
                }
            );
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
                (m_actors[i]->get_position().y - m_bounding_box.y) / m_bounding_box.height
            };
        }
    }


    void ContainerActor::add_actor_at_relative_position(BoundedBoxActor *actor, const Vector2 &relative_position) {
        m_actors.push_back(actor);
        m_actor_relative_positions.push_back(relative_position);
        auto anchored_actor_position = actor->get_anchored_position();
        m_bounding_box.x = std::min(m_bounding_box.x, anchored_actor_position.x);
        m_bounding_box.y = std::min(m_bounding_box.y, anchored_actor_position.y);
        m_bounding_box.width = std::max(m_bounding_box.width, anchored_actor_position.x + actor->get_width());
        m_bounding_box.height = std::max(m_bounding_box.height, anchored_actor_position.y + actor->get_height());
        update_relative_positions();
    }

    void ContainerActor::remove_actor(const BoundedBoxActor *actor) {
        throw std::runtime_error("Not implemented");
    }


    void ContainerActor::update(float deltaTime) {
        for (const auto &actor: m_actors) {
            actor->update(deltaTime);
        }
    }

    void ContainerActor::render() const {
        for (const auto &actor: m_actors) {
            actor->render();
        }
    }

    const Vector2 &ContainerActor::get_position() const {
        return m_position;
    }

    float ContainerActor::get_width() const {
        return m_bounding_box.width;
    }

    float ContainerActor::get_height() const {
        return m_bounding_box.height;
    }

    const Vector2 &ContainerActor::get_anchor() const {
        return m_anchor;
    }

    void ContainerActor::set_anchor(const Vector2 &anchor) {
        m_anchor = anchor;
        recalculate_bounding_box();
    }

    void ContainerActor::set_anchor(SpriteAnchor anchor) {
        switch (anchor) {
            case SpriteAnchor::TOP_LEFT:
                m_anchor = {0.0f, 0.0f};
                break;
            case SpriteAnchor::TOP_CENTER:
                m_anchor = {0.5f, 0.0f};
                break;
            case SpriteAnchor::TOP_RIGHT:
                m_anchor = {1.0f, 0.0f};
                break;
            case SpriteAnchor::MIDDLE_LEFT:
                m_anchor = {0.0f, 0.5f};
                break;
            case SpriteAnchor::MIDDLE_CENTER:
                m_anchor = {0.5f, 0.5f};
                break;
            case SpriteAnchor::MIDDLE_RIGHT:
                m_anchor = {1.0f, 0.5f};
                break;
            case SpriteAnchor::BOTTOM_LEFT:
                m_anchor = {0.0f, 1.0f};
                break;
            case SpriteAnchor::BOTTOM_CENTER:
                m_anchor = {0.5f, 1.0f};
                break;
            case SpriteAnchor::BOTTOM_RIGHT:
                m_anchor = {1.0f, 1.0f};
                break;
            default: ;
        }

        recalculate_bounding_box();
    }

    void ContainerActor::set_position(const Vector2 &position) {
        m_position = position;
        recalculate_bounding_box();
    }

    SpriteActor::SpriteActor(Vector2 position) : m_position(position) {
    }

    SpriteActor::SpriteActor(const Texture2D &texture, Vector2 position, SpriteAnchor anchor) : m_position(position),
        texture(texture) {
        SpriteActor::set_anchor(anchor);
    }

    void SpriteActor::set_position(const Vector2 &position) { this->m_position = position; }

    const Vector2 &SpriteActor::get_position() const { return m_position; }

    void SpriteActor::set_scale(float scale) { this->m_scale = scale; }

    float SpriteActor::get_scale() const { return m_scale; }

    void SpriteActor::set_anchor(SpriteAnchor anchor) {
        switch (anchor) {
            case SpriteAnchor::TOP_LEFT:
                this->m_anchor = {0.0f, 0.0f};
                break;
            case SpriteAnchor::TOP_CENTER:
                this->m_anchor = {0.5f, 0.0f};
                break;
            case SpriteAnchor::TOP_RIGHT:
                this->m_anchor = {1.0f, 0.0f};
                break;
            case SpriteAnchor::MIDDLE_LEFT:
                this->m_anchor = {0.0f, 0.5f};
                break;
            case SpriteAnchor::MIDDLE_CENTER:
                this->m_anchor = {0.5f, 0.5f};
                break;
            case SpriteAnchor::MIDDLE_RIGHT:
                this->m_anchor = {1.0f, 0.5f};
                break;
            case SpriteAnchor::BOTTOM_LEFT:
                this->m_anchor = {0.0f, 1.0f};
                break;
            case SpriteAnchor::BOTTOM_CENTER:
                this->m_anchor = {0.5f, 1.0f};
                break;
            case SpriteAnchor::BOTTOM_RIGHT:
                this->m_anchor = {1.0f, 1.0f};
                break;
            default: ;
        }
    }

    float SpriteActor::get_width() const { return static_cast<float>(texture.width) * m_scale; }

    float SpriteActor::get_height() const { return static_cast<float>(texture.height) * m_scale; }

    void SpriteActor::update(float deltaTime) {
    }

    void SpriteActor::render() const {
        const Vector2 render_position = {
            m_position.x - m_anchor.x * static_cast<float>(texture.width) * m_scale,
            m_position.y - m_anchor.y * static_cast<float>(texture.height) * m_scale

        };
        DrawTextureEx(texture, render_position, 0.0f, m_scale, WHITE);
    }

    Vector2 SpriteActor::get_anchored_position() const {
        return Vector2Add(m_position, Vector2Multiply(m_anchor, Vector2{get_width(), get_height()}));
    }

    const Vector2 &SpriteActor::get_anchor() const {
        return m_anchor;
    }

    void SpriteActor::set_anchor(const Vector2 &anchor) {
        m_anchor = anchor;
    }

    void GroupSpriteActor::update_bounding_box(Rectangle child_bounding_box) {
        m_bounding_box = {
            .x = std::min(m_bounding_box.x, child_bounding_box.x),
            .y = std::min(m_bounding_box.y, child_bounding_box.y),
            .width = std::max(m_bounding_box.width, child_bounding_box.width + child_bounding_box.x),
            .height = std::max(m_bounding_box.height, child_bounding_box.height + child_bounding_box.y)
        };
    }

    void GroupSpriteActor::update_children_positions() const {
        for (int i = 0; i < m_children.size(); i++) {
            m_children[i]->set_position({
                m_position.x + m_children_anchor_positions[i].x * m_bounding_box.width,
                m_position.y + m_children_anchor_positions[i].y * m_bounding_box.height
            });
        }
    }

    GroupSpriteActor::GroupSpriteActor(Vector2 position,
                                       const std::vector<SpriteActor *> &sprite_actors) : SpriteActor(position),
        m_children(sprite_actors) {
        for (const auto &child: m_children) {
            update_bounding_box(Rectangle{
                .x = child->get_position().x,
                .y = child->get_position().y,
                .width = child->get_width(),
                .height = child->get_height()
            });
        }
        for (const auto &child: m_children) {
            Vector2 child_anchor_position = {
                .x = (child->get_position().x - m_bounding_box.x) / m_bounding_box.width,
                .y = (child->get_position().y - m_bounding_box.y) / m_bounding_box.height
            };
            m_children_anchor_positions.push_back(child_anchor_position);
        }
    }

    GroupSpriteActor::~GroupSpriteActor() = default;

    void GroupSpriteActor::add_child(SpriteActor *child) {
        m_children.push_back(child);
        update_bounding_box(Rectangle{
            .x = child->get_position().x,
            .y = child->get_position().y,
            .width = child->get_width(),
            .height = child->get_height()
        });
        Vector2 child_anchor_position = {
            .x = (child->get_position().x - m_bounding_box.x) / m_bounding_box.width,
            .y = (child->get_position().y - m_bounding_box.y) / m_bounding_box.height
        };
        m_children_anchor_positions.push_back(child_anchor_position);
        update_children_positions();
    }

    void GroupSpriteActor::remove_child(const SpriteActor *child) { throw std::runtime_error("Not implemented"); }

    void GroupSpriteActor::render() const {
        for (const auto &child: m_children) {
            child->render();
        }
    }

    void GroupSpriteActor::set_position(const Vector2 &position) {
        m_position = {position.x - get_width() * m_anchor.x, position.y - get_height() * m_anchor.y};
        update_children_positions();
    }

    void GroupSpriteActor::set_scale(float scale) { throw std::runtime_error("Not implemented"); }

    float GroupSpriteActor::get_scale() const { throw std::runtime_error("Not implemented"); }

    void GroupSpriteActor::set_anchor(SpriteAnchor anchor) { SpriteActor::set_anchor(anchor); }

    float GroupSpriteActor::get_width() const { return m_bounding_box.width; }

    float GroupSpriteActor::get_height() const { return m_bounding_box.height; }
} // namespace Engine
