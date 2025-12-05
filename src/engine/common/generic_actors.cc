//
// Created by Ion Plamadeala on 23/11/2025.
//
#include "generic_actors.hh"

#include "raylib.h"
#include "raymath.h"

namespace Engine {
    void IClickableActor::on_mouse_entered(const Vector2 &mouse_position) {
    }

    void IClickableActor::on_mouse_exited(const Vector2 &mouse_position) {
    }

    void IClickableActor::on_mouse_pressed(const Vector2 &mouse_position) {
    }

    void IClickableActor::on_mouse_released(const Vector2 &mouse_position) {
    }

    auto PositionedActor::get_position() const -> Vector2 const & {
        return m_position;
    }

    void PositionedActor::set_position(const Vector2 &position) { m_position = position; }

    PositionedActor::PositionedActor(const Vector2 &position) : m_position(position) {
    }

    void BoundedBoxActor::update_bounding_box() {
        m_bounding_box.x = get_position().x - m_anchor.x * m_bounding_box.width;
        m_bounding_box.y = get_position().y - m_anchor.y * m_bounding_box.height;
    }

    auto BoundedBoxActor::get_anchored_position() const -> Vector2 {
        return Vector2Subtract(get_position(), Vector2Multiply(m_anchor, get_size()));
    }

    Vector2 BoundedBoxActor::get_size() const { return {.x = m_bounding_box.width, .y = m_bounding_box.height}; }

    const Vector2 &BoundedBoxActor::get_anchor() const { return m_anchor; }

    void BoundedBoxActor::set_anchor(const Vector2 &anchor) { m_anchor = anchor; }

    void BoundedBoxActor::set_anchor(AlignmentAnchor anchor) {
        switch (anchor) {
            case AlignmentAnchor::TOP_LEFT:
                m_anchor = {0.0f, 0.0f};
                break;
            case AlignmentAnchor::TOP_CENTER:
                m_anchor = {0.5f, 0.0f};
                break;
            case AlignmentAnchor::TOP_RIGHT:
                m_anchor = {1.0f, 0.0f};
                break;
            case AlignmentAnchor::MIDDLE_LEFT:
                m_anchor = {0.0f, 0.5f};
                break;
            case AlignmentAnchor::MIDDLE_CENTER:
                m_anchor = {0.5f, 0.5f};
                break;
            case AlignmentAnchor::MIDDLE_RIGHT:
                m_anchor = {1.0f, 0.5f};
                break;
            case AlignmentAnchor::BOTTOM_LEFT:
                m_anchor = {0.0f, 1.0f};
                break;
            case AlignmentAnchor::BOTTOM_CENTER:
                m_anchor = {0.5f, 1.0f};
                break;
            case AlignmentAnchor::BOTTOM_RIGHT:
                m_anchor = {1.0f, 1.0f};
                break;
            default: ;
        }
    }

    void BoundedBoxActor::set_bounding_box(const Rectangle &bounding_box) { m_bounding_box = bounding_box; }

    void BoundedBoxActor::set_position(const Vector2 &position) {
        PositionedActor::set_position(position);
        m_bounding_box.x = get_position().x - m_anchor.x * m_bounding_box.width;
        m_bounding_box.y = get_position().y - m_anchor.y * m_bounding_box.height;
    }

    const Rectangle &BoundedBoxActor::get_bounding_box() const { return m_bounding_box; }

    bool BoundedBoxActor::is_mouse_over(const Vector2 &mouse_position) {
        bool is_mouse_inside = CheckCollisionPointRec(mouse_position, m_bounding_box);
        if (is_mouse_inside && !m_is_mouse_inside) {
            m_is_mouse_inside = true;
            on_mouse_entered(mouse_position);
        } else if (!is_mouse_inside && m_is_mouse_inside) {
            m_is_mouse_inside = false;
            on_mouse_exited(mouse_position);
        }
        return is_mouse_inside;
    }

    void BoundedBoxActor::update(float deltaTime) {
    }

    void BoundedBoxActor::render() const {
    }


    [[nodiscard]] auto IActor::get_z_index() const -> RenderLayer { return m_z_index; }
    void IActor::set_z_index(RenderLayer layer) { m_z_index = layer; }

    auto IActor::get_bucket_index() const -> size_t { return m_z_bucket_index; }

    void IActor::set_bucket_index(size_t index) { m_z_bucket_index = index; }

    auto IActor::is_visible() const -> bool { return m_visible; }

    void IActor::set_visible(bool visible) { m_visible = visible; }
} // namespace Engine
