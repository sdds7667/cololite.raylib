
#pragma once
#include <cstdint>
#include "generic_actors.hh"

namespace Engine {

    struct Scene {
    private:
        static constexpr size_t num_layers = static_cast<size_t>(RenderLayer::COUNT);
        LayeredContainer container;

    public:
        // O(1) add to specific layer
        void add_actor(IActor *actor, RenderLayer layer) { container.add(actor, layer); }

        // O(1) swap-remove
        void remove_actor(IActor *actor) { container.remove(actor); }

        // O(1) layer change
        void change_layer(IActor *actor, RenderLayer new_layer) { container.change_layer(actor, new_layer); }

        void update_all(float delta_time) {
            container.for_each([delta_time](IActor *actor) { actor->update(delta_time); });
        }

        void render_all() const {
            container.for_each([](const IActor *actor) {
                if (actor->is_visible()) {
                    actor->render();
                }
            });
        }

        // Query by layer
        const std::vector<IActor *> &get_layer(RenderLayer layer) const { return container.get_layer(layer); }

        std::vector<IActor *> &get_layer(RenderLayer layer) { return container.get_layer(layer); }

        // Legacy compatibility - returns all actors in z-order
        auto get_all_actors() const -> stdr::view auto { return container.get_all(); }

        void clear() { container.clear(); }

        size_t get_total_actor_count() const { return container.get_total_count(); }
    };
} // namespace Engine
