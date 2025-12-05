//
// Created by Ion Plamadeala on 23/11/2025.
//

#ifndef COLOLITE_GENERIC_ACTORS_HH
#define COLOLITE_GENERIC_ACTORS_HH

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <vector>
#include "raylib.h"

namespace stdr = std::ranges;
namespace stdv = std::ranges::views;

namespace Engine {

    enum class RenderLayer : std::uint8_t {
        UNDEFINED = 0,
        MAP_TERRAIN,
        MAP_EDGES,
        MAP_CORNERS,
        MAP_HIGHLIGHTS,
        GAME_PIECES,
        UI_PANELS,
        UI_INTERACTIVE,
        DRAG_DROP,
        TOOLTIPS,
        DEBUG,
        COUNT
    };


    class IActor {
        bool m_visible = true;
        size_t m_z_bucket_index = 0; // Index within the layer bucket
        RenderLayer m_z_index = RenderLayer::UNDEFINED; // Layer index
    private:
        [[nodiscard]] auto get_bucket_index() const -> size_t;

        void set_bucket_index(size_t index);

        [[nodiscard]] auto get_z_index() const -> RenderLayer;
        void set_z_index(RenderLayer layer);

    public:
        virtual ~IActor() = default;

        virtual void update(float deltaTime) = 0;

        virtual void render() const = 0;

        [[nodiscard]] auto is_visible() const -> bool;

        void set_visible(bool visible);
        friend class LayeredContainer;
    };

    class LayeredContainer {
    private:
        std::array<std::vector<IActor *>, static_cast<size_t>(RenderLayer::COUNT)> layers;

    public:
        void add(IActor *item, RenderLayer layer) {
            if (layer == RenderLayer::UNDEFINED) {
                return;
            }

            item->set_z_index(layer);
            auto &bucket = get_layer(layer);
            item->set_bucket_index(bucket.size());
            bucket.push_back(item);
        }

        void remove(IActor *item) {
            auto layer = item->get_z_index();
            if (layer == RenderLayer::UNDEFINED) {
                return;
            }
            auto &bucket = get_layer(layer);
            size_t idx = item->get_bucket_index();

            if (idx >= bucket.size()) {
                return;
            }

            // Swap with last element
            if (idx < bucket.size() - 1) {
                IActor *last_item = bucket.back();
                bucket[idx] = last_item;
                last_item->set_bucket_index(idx); // Update swapped item's index
            }

            bucket.pop_back();
        }

        void change_layer(IActor *item, RenderLayer new_layer) {
            remove(item);
            add(item, new_layer);
        }

        template<typename Func>
        void for_each(Func &&func) {
            for (auto &bucket: layers) {
                for (auto *item: bucket) {
                    func(item);
                }
            }
        }

        template<typename Func>
        void for_each(Func &&func) const {
            for (const auto &bucket: layers) {
                for (const auto *item: bucket) {
                    func(item);
                }
            }
        }

        // Query by layer
        [[nodiscard]] auto get_layer(RenderLayer layer) const -> std::vector<IActor *> const & {
            // Layer is guaranteed to be here as it is part of the enum
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
            return layers[static_cast<int>(layer)];
        }

        auto get_layer(RenderLayer layer) -> std::vector<IActor *> & {
            // Layer is guaranteed to be here as it is part of the enum
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index
            return layers[static_cast<int>(layer)];
        }

        // Get all items in z-order
        [[nodiscard]] auto get_all() const -> stdr::view auto { return layers | stdv::join; }


        void clear() {
            for (auto &bucket: layers) {
                bucket.clear();
            }
        }

        [[nodiscard]] auto get_total_count() const -> size_t {
            auto range = get_all();
            return stdr::distance(range.begin(), range.end());
        }

        [[nodiscard]] auto empty() const -> bool {
            return stdr::any_of(layers, [](const auto &bucket) { return !bucket.empty(); });
        }
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
} // namespace Engine

#endif // COLOLITE_GENERIC_ACTORS_HH
