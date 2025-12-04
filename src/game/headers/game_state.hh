
#pragma once

#include <deque>
#include "game.hh"
#include "game_sequence.hh"
#include "map.hh"
namespace Game {

    struct RollManager {
        std::deque<Roll> rolls_queue;
        std::vector<Roll> discard_pile;

        RollManager() = default;

        void initialize_rolls();
    };

    using PlayerResources = std::unordered_map<Map::Resource, int>;

    class GameState {
        GameSequence m_game_sequence;
        RollManager m_roll_manager;
        PlayerResources m_player_resources;
        Map::Map m_map;
        Map::Corner *m_last_built_corner = nullptr;
        GameState(Map::Map map);

    public:
        // Trade
        auto can_trade(Map::Resource &resource_to_be_sold) const -> bool;
        auto can_trade(Map::Resource &resource_to_be_sold, Map::Resource &resource_to_be_bought) const -> int;
        void apply_trade(Map::Resource &resource_to_be_sold, Map::Resource &resource_to_be_bought, int quantity);

        [[nodiscard]] auto get_map() const -> const Map::Map &;

        friend auto get_game_state() -> GameState &;
    };

    auto get_game_state() -> GameState &;


} // namespace Game
