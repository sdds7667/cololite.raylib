
#include "map_actor.hh"


namespace GameActors {
    MapActor::MapActor(const Vector2 &position, const Map::Map &map) : PositionedActor(position), m_map(map) {
    }

    auto MapActor::get_children() -> LayeredContainer & {
        return m_actors;
    }

    auto MapActor::get_children() const -> const LayeredContainer & {
        return m_actors;
    }


    void MapActor::update(float delta_time) {
        for (const auto &actor: m_actors.get_all()) {
            actor->update(delta_time);
        }
    }

    void MapActor::render() const {
        for (const auto &actor: m_actors.get_all()) {
            actor->render();
        }
    }

    auto MapActor::is_mouse_over(const Vector2 &mouse_position) -> bool {
        return false;
    }

    void MapActor::on_mouse_entered(const Vector2 &mouse_position) {
        IClickableActor::on_mouse_entered(mouse_position);
    }

    void MapActor::on_mouse_exited(const Vector2 &mouse_position) {
        IClickableActor::on_mouse_exited(mouse_position);
    }

    void MapActor::on_mouse_pressed(const Vector2 &mouse_position) {
        IClickableActor::on_mouse_pressed(mouse_position);
    }

    void MapActor::on_mouse_released(const Vector2 &mouse_position) {
        IClickableActor::on_mouse_released(mouse_position);
    }
}
