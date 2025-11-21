//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ACTOR_HH
#define COLOLITE_ACTOR_HH
#include <raylib.h>
#include <vector>

namespace Engine {
    class Actor {
    public:
        virtual ~Actor() = default;

        virtual void update(float deltaTime) = 0;

        virtual void render() const = 0;
    };

    class ClickableActor {
    public:
        virtual ~ClickableActor() = default;

        [[nodiscard]] virtual bool is_clicked(const Vector2 &mouse_position) const = 0;
    };

    enum class SpriteAnchor {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        MIDDLE_CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
        CUSTOM
    };

    class BoundedBoxActor : public Actor {
    public:
        virtual ~BoundedBoxActor() = default;

        [[nodiscard]] virtual const Vector2 &get_position() const = 0;

        [[nodiscard]] virtual Vector2 get_anchored_position() const = 0;

        [[nodiscard]] virtual float get_width() const = 0;

        [[nodiscard]] virtual float get_height() const = 0;

        [[nodiscard]] virtual const Vector2 &get_anchor() const = 0;

        virtual void set_anchor(const Vector2 &anchor) = 0;

        virtual void set_anchor(SpriteAnchor anchor) = 0;

        virtual void set_position(const Vector2 &position) = 0;
    };

    class ContainerActor : public BoundedBoxActor {
    protected:
        std::vector<BoundedBoxActor *> m_actors{};
        std::vector<Vector2> m_actor_relative_positions{};
        Rectangle m_bounding_box{};
        Vector2 m_anchor{};
        Vector2 m_position{};

        void update_children_positions() const;

        void recalculate_bounding_box();

        void update_relative_positions();

    public:
        bool changes_propagated_to_children = true;


        void add_actor_at_relative_position(BoundedBoxActor *actor, const Vector2 &relative_position);

        void remove_actor(const BoundedBoxActor *actor);

        void update(float deltaTime) override;

        void render() const override;


        [[nodiscard]] const Vector2 &get_position() const override;

        [[nodiscard]] float get_width() const override;

        [[nodiscard]] float get_height() const override;

        [[nodiscard]] const Vector2 &get_anchor() const override;

        void set_anchor(const Vector2 &anchor) override;

        void set_anchor(SpriteAnchor anchor) override;

        void set_position(const Vector2 &position) override;
    };


    class SpriteActor : public BoundedBoxActor {
    protected:
        Vector2 m_position{};
        float m_scale = 1.0f;
        Vector2 m_anchor{};
        Texture2D texture{};

    public:
        explicit SpriteActor(Vector2 position);

        SpriteActor(const Texture2D &texture, Vector2 position, SpriteAnchor anchor = SpriteAnchor::TOP_LEFT);

        [[nodiscard]] const Vector2 &get_position() const override;

        virtual void set_scale(float scale);

        [[nodiscard]] virtual float get_scale() const;

        void set_anchor(SpriteAnchor anchor) override;

        [[nodiscard]] float get_width() const override;

        [[nodiscard]] float get_height() const override;

        void update(float deltaTime) override;

        void render() const override;

        [[nodiscard]] Vector2 get_anchored_position() const override;

        [[nodiscard]] const Vector2 &get_anchor() const override;

        void set_anchor(const Vector2 &anchor) override;

        void set_position(const Vector2 &position) override;
    };

    class GroupSpriteActor : public SpriteActor {
        std::vector<SpriteActor *> m_children{};
        std::vector<Vector2> m_children_anchor_positions{};
        Rectangle m_bounding_box{};

    protected:
        void update_bounding_box(Rectangle child_bounding_box);

        void update_children_positions() const;

    public:
        GroupSpriteActor(Vector2 position, const std::vector<SpriteActor *> &sprite_actors);

        ~GroupSpriteActor() override;

        void add_child(SpriteActor *child);

        void remove_child(const SpriteActor *child);

        void render() const override;

        void set_position(const Vector2 &position) override;

        void set_scale(float scale) override;

        [[nodiscard]] float get_scale() const override;

        void set_anchor(SpriteAnchor anchor) override;

        [[nodiscard]] float get_width() const override;

        [[nodiscard]] float get_height() const override;
    };

    class ButtonActor : public Actor, public ClickableActor {
        SpriteActor *m_sprite_actor;

    public:
        void update(float deltaTime) override;

        void render() const override;

        [[nodiscard]] bool is_clicked(const Vector2 &mouse_position) const override;
    };
} // namespace Engine

#endif // COLOLITE_ACTOR_HH
