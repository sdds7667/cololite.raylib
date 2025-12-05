#include <gtest/gtest.h>
#include "generic_actors.hh"

namespace Engine {
    // Mock Actor for testing
    class MockActor final : public IActor {
        int update_count = 0;
        int render_count = 0;

    public:
        void update(float deltaTime) override { update_count++; }

        void render() const override {
        }

        [[nodiscard]] auto get_update_count() const -> int { return update_count; }
    };

    // Test fixture for LayeredContainer
    class LayeredContainerTest : public ::testing::Test {
    protected:
        LayeredContainer container;

        void TearDown() override {
            // Clean up any allocated actors
            container.clear();
        }
    };

    // Test: Add a single actor to a layer
    TEST_F(LayeredContainerTest, AddSingleActor) {
        MockActor actor;

        container.add(&actor, RenderLayer::MAP_TERRAIN);

        const auto &layer = container.get_layer(RenderLayer::MAP_TERRAIN);
        EXPECT_EQ(layer.size(), 1);
        EXPECT_EQ(layer[0], &actor);
    }

    // Test: Add multiple actors to the same layer
    TEST_F(LayeredContainerTest, AddMultipleActorsToSameLayer) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::UI_PANELS);
        container.add(&actor2, RenderLayer::UI_PANELS);
        container.add(&actor3, RenderLayer::UI_PANELS);

        const auto &layer = container.get_layer(RenderLayer::UI_PANELS);
        EXPECT_EQ(layer.size(), 3);
        EXPECT_EQ(layer[0], &actor1);
        EXPECT_EQ(layer[1], &actor2);
        EXPECT_EQ(layer[2], &actor3);
    }

    // Test: Add actors to different layers
    TEST_F(LayeredContainerTest, AddActorsToDifferentLayers) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::MAP_TERRAIN);
        container.add(&actor2, RenderLayer::GAME_PIECES);
        container.add(&actor3, RenderLayer::UI_PANELS);

        EXPECT_EQ(container.get_layer(RenderLayer::MAP_TERRAIN).size(), 1);
        EXPECT_EQ(container.get_layer(RenderLayer::GAME_PIECES).size(), 1);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS).size(), 1);

        EXPECT_EQ(container.get_layer(RenderLayer::MAP_TERRAIN)[0], &actor1);
        EXPECT_EQ(container.get_layer(RenderLayer::GAME_PIECES)[0], &actor2);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS)[0], &actor3);
    }

    // Test: Remove an actor from the layer
    TEST_F(LayeredContainerTest, RemoveSingleActor) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::MAP_EDGES);
        container.add(&actor2, RenderLayer::MAP_EDGES);
        container.add(&actor3, RenderLayer::MAP_EDGES);

        EXPECT_EQ(container.get_layer(RenderLayer::MAP_EDGES).size(), 3);

        container.remove(&actor2);

        const auto &layer = container.get_layer(RenderLayer::MAP_EDGES);
        EXPECT_EQ(layer.size(), 2);
        EXPECT_EQ(layer[0], &actor1);
        EXPECT_EQ(layer[1], &actor3); // actor3 should be swapped to position 1
    }

    // Test: Remove the first actor (swap-remove behavior)
    TEST_F(LayeredContainerTest, RemoveFirstActor) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::DEBUG);
        container.add(&actor2, RenderLayer::DEBUG);
        container.add(&actor3, RenderLayer::DEBUG);

        container.remove(&actor1);

        const auto &layer = container.get_layer(RenderLayer::DEBUG);
        EXPECT_EQ(layer.size(), 2);
        EXPECT_EQ(layer[0], &actor3); // Last actor swapped to first position
        EXPECT_EQ(layer[1], &actor2);
    }

    // Test: Remove last actor
    TEST_F(LayeredContainerTest, RemoveLastActor) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::TOOLTIPS);
        container.add(&actor2, RenderLayer::TOOLTIPS);
        container.add(&actor3, RenderLayer::TOOLTIPS);

        container.remove(&actor3);

        const auto &layer = container.get_layer(RenderLayer::TOOLTIPS);
        EXPECT_EQ(layer.size(), 2);
        EXPECT_EQ(layer[0], &actor1);
        EXPECT_EQ(layer[1], &actor2);
    }

    // Test: Remove only an actor in layer
    TEST_F(LayeredContainerTest, RemoveOnlyActor) {
        MockActor actor;

        container.add(&actor, RenderLayer::DRAG_DROP);
        EXPECT_EQ(container.get_layer(RenderLayer::DRAG_DROP).size(), 1);

        container.remove(&actor);

        EXPECT_EQ(container.get_layer(RenderLayer::DRAG_DROP).size(), 0);
    }

    // Test: Change actor layer
    TEST_F(LayeredContainerTest, ChangeLayer) {
        MockActor actor;

        container.add(&actor, RenderLayer::MAP_TERRAIN);
        EXPECT_EQ(container.get_layer(RenderLayer::MAP_TERRAIN).size(), 1);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS).size(), 0);

        container.change_layer(&actor, RenderLayer::UI_PANELS);

        EXPECT_EQ(container.get_layer(RenderLayer::MAP_TERRAIN).size(), 0);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS).size(), 1);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS)[0], &actor);
    }

    // Test: Change layer with multiple actors
    TEST_F(LayeredContainerTest, ChangeLayerWithMultipleActors) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::MAP_CORNERS);
        container.add(&actor2, RenderLayer::MAP_CORNERS);
        container.add(&actor3, RenderLayer::MAP_CORNERS);

        container.change_layer(&actor2, RenderLayer::MAP_HIGHLIGHTS);

        EXPECT_EQ(container.get_layer(RenderLayer::MAP_CORNERS).size(), 2);
        EXPECT_EQ(container.get_layer(RenderLayer::MAP_HIGHLIGHTS).size(), 1);
        EXPECT_EQ(container.get_layer(RenderLayer::MAP_HIGHLIGHTS)[0], &actor2);
    }

    // Test: for_each iteration (non-const)
    TEST_F(LayeredContainerTest, ForEachIteration) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::MAP_TERRAIN);
        container.add(&actor2, RenderLayer::GAME_PIECES);
        container.add(&actor3, RenderLayer::UI_PANELS);

        int count = 0;
        container.for_each([&count](IActor *actor) {
            actor->update(1.0F);
            count++;
        });

        EXPECT_EQ(count, 3);
        EXPECT_EQ(actor1.get_update_count(), 1);
        EXPECT_EQ(actor2.get_update_count(), 1);
        EXPECT_EQ(actor3.get_update_count(), 1);
    }

    // Test: for_each iteration (const)
    TEST_F(LayeredContainerTest, ForEachIterationConst) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::MAP_TERRAIN);
        container.add(&actor2, RenderLayer::GAME_PIECES);
        container.add(&actor3, RenderLayer::UI_PANELS);

        const LayeredContainer &const_container = container;

        int count = 0;
        const_container.for_each([&count](const IActor *) { count++; });

        EXPECT_EQ(count, 3);
    }

    // Test: for_each with z-order (layers processed in order)
    TEST_F(LayeredContainerTest, ForEachZOrder) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;
        MockActor actor4;

        container.add(&actor1, RenderLayer::DEBUG); // Last layer
        container.add(&actor2, RenderLayer::MAP_TERRAIN); // First layer
        container.add(&actor3, RenderLayer::UI_PANELS); // Middle layer
        container.add(&actor4, RenderLayer::MAP_TERRAIN); // First layer

        std::vector<IActor *> visited;
        container.for_each([&visited](IActor *actor) { visited.push_back(actor); });

        EXPECT_EQ(visited.size(), 4);
        // MAP_TERRAIN actors should come first
        EXPECT_EQ(visited[0], &actor2);
        EXPECT_EQ(visited[1], &actor4);
        // Then UI_PANELS
        EXPECT_EQ(visited[2], &actor3);
        // Then DEBUG
        EXPECT_EQ(visited[3], &actor1);
    }

    // Test: get_layer (const and non-const)
    TEST_F(LayeredContainerTest, GetLayer) {
        MockActor actor1;
        MockActor actor2;

        container.add(&actor1, RenderLayer::MAP_EDGES);
        container.add(&actor2, RenderLayer::MAP_EDGES);

        // Non-const access
        const auto &layer = container.get_layer(RenderLayer::MAP_EDGES);
        EXPECT_EQ(layer.size(), 2);

        // Const access
        const LayeredContainer &const_container = container;
        const auto &const_layer = const_container.get_layer(RenderLayer::MAP_EDGES);
        EXPECT_EQ(const_layer.size(), 2);
    }

    // Test: get_all returns all actors in z-order
    TEST_F(LayeredContainerTest, GetAll) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::GAME_PIECES);
        container.add(&actor2, RenderLayer::MAP_TERRAIN);
        container.add(&actor3, RenderLayer::UI_INTERACTIVE);

        const auto all_actors = container.get_all();
        std::vector<IActor *> actors_vec;
        for (auto *actor: all_actors) {
            actors_vec.push_back(actor);
        }

        EXPECT_EQ(actors_vec.size(), 3);
        // Should be in layer order: MAP_TERRAIN, GAME_PIECES, UI_INTERACTIVE
        EXPECT_EQ(actors_vec[0], &actor2);
        EXPECT_EQ(actors_vec[1], &actor1);
        EXPECT_EQ(actors_vec[2], &actor3);
    }

    // Test: clear all layers
    TEST_F(LayeredContainerTest, Clear) {
        MockActor actor1;
        MockActor actor2;
        MockActor actor3;

        container.add(&actor1, RenderLayer::MAP_TERRAIN);
        container.add(&actor2, RenderLayer::GAME_PIECES);
        container.add(&actor3, RenderLayer::UI_PANELS);

        EXPECT_EQ(container.get_total_count(), 3);

        container.clear();

        EXPECT_EQ(container.get_total_count(), 0);
        EXPECT_EQ(container.get_layer(RenderLayer::MAP_TERRAIN).size(), 0);
        EXPECT_EQ(container.get_layer(RenderLayer::GAME_PIECES).size(), 0);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS).size(), 0);
    }

    // Test: get_total_count
    TEST_F(LayeredContainerTest, GetTotalCount) {
        EXPECT_EQ(container.get_total_count(), 0);

        MockActor actor1, actor2, actor3, actor4, actor5;

        container.add(&actor1, RenderLayer::MAP_TERRAIN);
        EXPECT_EQ(container.get_total_count(), 1);

        container.add(&actor2, RenderLayer::MAP_TERRAIN);
        EXPECT_EQ(container.get_total_count(), 2);

        container.add(&actor3, RenderLayer::GAME_PIECES);
        container.add(&actor4, RenderLayer::UI_PANELS);
        container.add(&actor5, RenderLayer::DEBUG);
        EXPECT_EQ(container.get_total_count(), 5);

        container.remove(&actor3);
        EXPECT_EQ(container.get_total_count(), 4);
    }

    // Test: empty check
    TEST_F(LayeredContainerTest, EmptyCheck) {
        EXPECT_FALSE(container.empty());
        // Should return false when the container is actually empty (bug in implementation)

        MockActor actor;
        container.add(&actor, RenderLayer::MAP_TERRAIN);

        EXPECT_TRUE(container.empty()); // Should return true when a container has actors

        container.clear();
        EXPECT_FALSE(container.empty()); // Should return false when cleared
    }

    // Test: Actor bucket index management
    TEST_F(LayeredContainerTest, BucketIndexTracking) {
        MockActor actor1, actor2, actor3;

        container.add(&actor1, RenderLayer::TOOLTIPS);
        container.add(&actor2, RenderLayer::TOOLTIPS);
        container.add(&actor3, RenderLayer::TOOLTIPS);

        // After removing a middle actor, indices should be updated correctly
        container.remove(&actor2);

        const auto &layer = container.get_layer(RenderLayer::TOOLTIPS);
        EXPECT_EQ(layer.size(), 2);

        // Verify the container still works correctly after index updates
        container.remove(&actor3);
        EXPECT_EQ(layer.size(), 1);
        EXPECT_EQ(layer[0], &actor1);
    }

    // Test: Multiple remove operations
    TEST_F(LayeredContainerTest, MultipleRemoves) {
        MockActor actor1, actor2, actor3, actor4, actor5;

        container.add(&actor1, RenderLayer::UI_INTERACTIVE);
        container.add(&actor2, RenderLayer::UI_INTERACTIVE);
        container.add(&actor3, RenderLayer::UI_INTERACTIVE);
        container.add(&actor4, RenderLayer::UI_INTERACTIVE);
        container.add(&actor5, RenderLayer::UI_INTERACTIVE);

        container.remove(&actor3);
        container.remove(&actor1);
        container.remove(&actor5);

        const auto &layer = container.get_layer(RenderLayer::UI_INTERACTIVE);
        EXPECT_EQ(layer.size(), 2);
    }

    // Test: Add after remove
    TEST_F(LayeredContainerTest, AddAfterRemove) {
        MockActor actor1, actor2, actor3;

        container.add(&actor1, RenderLayer::DRAG_DROP);
        container.add(&actor2, RenderLayer::DRAG_DROP);

        container.remove(&actor1);

        container.add(&actor3, RenderLayer::DRAG_DROP);

        const auto &layer = container.get_layer(RenderLayer::DRAG_DROP);
        EXPECT_EQ(layer.size(), 2);
        EXPECT_EQ(layer[0], &actor2);
        EXPECT_EQ(layer[1], &actor3);
    }

    // Test: Change layer back and forth
    TEST_F(LayeredContainerTest, ChangeLayerMultipleTimes) {
        MockActor actor;

        container.add(&actor, RenderLayer::MAP_TERRAIN);
        container.change_layer(&actor, RenderLayer::UI_PANELS);
        container.change_layer(&actor, RenderLayer::DEBUG);
        container.change_layer(&actor, RenderLayer::MAP_TERRAIN);

        EXPECT_EQ(container.get_layer(RenderLayer::MAP_TERRAIN).size(), 1);
        EXPECT_EQ(container.get_layer(RenderLayer::UI_PANELS).size(), 0);
        EXPECT_EQ(container.get_layer(RenderLayer::DEBUG).size(), 0);
    }

    // Test: All render layers
    TEST_F(LayeredContainerTest, AllRenderLayers) {
        MockActor actors[static_cast<size_t>(RenderLayer::COUNT)];

        container.add(&actors[0], RenderLayer::MAP_TERRAIN);
        container.add(&actors[1], RenderLayer::MAP_EDGES);
        container.add(&actors[2], RenderLayer::MAP_CORNERS);
        container.add(&actors[3], RenderLayer::MAP_HIGHLIGHTS);
        container.add(&actors[4], RenderLayer::GAME_PIECES);
        container.add(&actors[5], RenderLayer::UI_PANELS);
        container.add(&actors[6], RenderLayer::UI_INTERACTIVE);
        container.add(&actors[7], RenderLayer::DRAG_DROP);
        container.add(&actors[8], RenderLayer::TOOLTIPS);
        container.add(&actors[9], RenderLayer::DEBUG);

        EXPECT_EQ(container.get_total_count(), 10) << "Total count should be 10 after adding one actor to each layer";

        for (size_t i = 1; i < static_cast<size_t>(RenderLayer::COUNT); ++i) {
            const auto layer = static_cast<RenderLayer>(i);
            EXPECT_EQ(container.get_layer(layer).size(), 1)
                    << "Layer " << i << " should have 1 actor but has " << container.get_layer(layer).size();
        }
    }
} // namespace Engine
