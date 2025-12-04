
#include "game_state.hh"

namespace Game {

    GameState::GameState(Map::Map map) :
        m_map(std::move(map)),
        m_player_resources{
                {Map::Resource::WHEAT, 20}, {Map::Resource::WOOD, 20},  {Map::Resource::BRICK, 20},
                {Map::Resource::SHEEP, 20}, {Map::Resource::STONE, 20},
        } {
        m_roll_manager.initialize_rolls();
    }

    auto GameState::get_map() const -> const Map::Map & { return m_map; }

    auto get_game_state() -> GameState & {
        static GameState game_state(Map::Map::build_map_of_size(2));
        return game_state;
    }


} // namespace Game
