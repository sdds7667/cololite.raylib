//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ACTOR_HH
#define COLOLITE_ACTOR_HH
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include "generic_actors.hh"

#include "engine_settings.hh"
#include "map.hh"

namespace Engine {
    class FixedSizedTextActor : public BoundedBoxActor {
    protected:
        std::string m_text;
        const Font &m_font;
        Color m_color;
        float m_font_size;

    public:
        FixedSizedTextActor(const std::string &text, const Font &font, Color color, float font_size, Vector2 position);

        ~FixedSizedTextActor() override = default;

        void update(float deltaTime) override;

        void render() const override;

        void set_text(const std::string &text);

        void set_color(Color color);
    };

    class ContainerActor : public BoundedBoxActor {
    protected:
        std::vector<BoundedBoxActor *> m_actors{};
        std::vector<Vector2> m_actor_relative_positions{};
        std::optional<Color> m_background_color{};

        void update_children_positions() const;

        void recalculate_bounding_box();

        void update_relative_positions();

    public:
        ContainerActor(const Vector2 &position, const std::vector<BoundedBoxActor *> &actors);

        bool changes_propagated_to_children = true;

        void add_actor_at_relative_position(BoundedBoxActor *actor, const Vector2 &relative_position);

        void set_position(const Vector2 &position) override;

        void remove_actor(const BoundedBoxActor *actor);

        void set_background_color(const std::optional<Color> &color);

        void update(float deltaTime) override;

        void render() const override;

        void cleanup() const;
    };


    class SpriteActor : public BoundedBoxActor {
        float m_scale = 1.0f;
        Texture2D texture{};

    public:
        explicit SpriteActor(Vector2 position);

        SpriteActor(const Texture2D &texture, Vector2 position, AlignmentAnchor anchor = AlignmentAnchor::TOP_LEFT);

        virtual void set_scale(float scale);

        [[nodiscard]] virtual float get_scale() const;

        void update(float deltaTime) override;

        void render() const override;
    };

    class ResourceDisplayActor : public ContainerActor {
        std::unordered_map<Map::Resource, FixedSizedTextActor *> resource_text_actors;
        std::unordered_map<Map::Resource, FixedSizedTextActor *> delta_resources_actors;
        std::unordered_map<Map::Resource, BoundedBoxActor *> resource_bounding_box;
        std::unordered_map<Map::Resource, ContainerActor *> resource_sprites;

    public:
        ResourceDisplayActor(const RenderResources &render_resources,
                             const std::unordered_map<Map::Resource, int> &player_resources);

        ~ResourceDisplayActor() override = default;

        void update_resources(const std::unordered_map<Map::Resource, int> &player_resources,
                              const std::unordered_map<Map::Resource, int> &delta_resources) const;

        void render() const override;

        [[nodiscard]] std::optional<Map::Resource> is_over_resource(const Vector2 &mouse_position) const;

        [[nodiscard]] std::optional<Map::Resource> is_over_resource_sprite(const Vector2 &mouse_position) const;

        [[nodiscard]] ContainerActor *get_drag_and_drop_resource(const RenderResources &resources,
                                                                 const Vector2 &mouse_position) const;
    };
} // namespace Engine

#endif // COLOLITE_ACTOR_HH
