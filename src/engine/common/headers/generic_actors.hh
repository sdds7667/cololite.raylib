//
// Created by Ion Plamadeala on 23/11/2025.
//

#ifndef COLOLITE_GENERIC_ACTORS_HH
#define COLOLITE_GENERIC_ACTORS_HH

#include "raylib.h"

namespace Engine {
    class IActor {
        int m_z_index = 0;
        bool m_visible = true;

    public:
        virtual ~IActor() = default;

        virtual void update(float deltaTime) = 0;

        virtual void render() const = 0;

        [[nodiscard]] int get_z_index() const;

        void set_z_index(int z_index);

        [[nodiscard]] bool is_visible() const;

        void set_visible(bool visible);
    };

    class IClickableActor {
    public:
        virtual ~IClickableActor() = default;

        [[nodiscard]] virtual bool is_mouse_over(const Vector2 &mouse_position) = 0;

        virtual void on_mouse_entered(const Vector2 &mouse_position);

        virtual void on_mouse_exited(const Vector2 &mouse_position);

        virtual void on_mouse_pressed(const Vector2 &mouse_position);

        virtual void on_mouse_released(const Vector2 &mouse_position);
    };

    enum class AlignmentAnchor {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        MIDDLE_CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
    };

    class BoundedBoxActor : public IActor, public IClickableActor {
    protected:
        Vector2 m_anchor{};
        Vector2 m_position{};
        Rectangle m_bounding_box{};

        bool m_is_mouse_inside = false;

    public:
        ~BoundedBoxActor() override = default;

        [[nodiscard]] virtual const Vector2 &get_position() const;

        [[nodiscard]] virtual Vector2 get_anchored_position() const;

        [[nodiscard]] virtual Vector2 get_size() const;

        [[nodiscard]] virtual const Vector2 &get_anchor() const;

        virtual void set_anchor(const Vector2 &anchor);

        virtual void set_anchor(AlignmentAnchor anchor);

        virtual void set_bounding_box(const Rectangle &bounding_box);

        virtual void set_position(const Vector2 &position);

        [[nodiscard]] virtual const Rectangle &get_bounding_box() const;

        bool is_mouse_over(const Vector2 &mouse_position) override;

        void update(float deltaTime) override;

        void render() const override;
    };
}

#endif //COLOLITE_GENERIC_ACTORS_HH
