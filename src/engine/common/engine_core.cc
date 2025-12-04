//
// Created by Ion Plamadeala on 04/12/2025.
//

#include "engine_core.hh"
#include "engine_settings.hh"
#include "raylib.h"
#ifdef __APPLE__
#include "screensize.hh"
#endif

namespace Engine {
    void initialize() {
        float screen_width = 0;
        float screen_height = 0;

#ifdef __APPLE__
        Vector2 screen_size = MacOS::get_screen_size();
        screen_width = screen_size.x;
        screen_height = screen_size.y;
#endif
        InitWindow(static_cast<int>(screen_width), static_cast<int>(screen_height), "Cololite");
        SetTargetFPS(EngineSettingsSingleton::get_instance().get_render_settings().target_fps);
    }

    void update(float delta_time) {
        // Update logic can be added here
        Scene *current_scene = EngineSettingsSingleton::get_instance().get_current_scene();
        if (current_scene != nullptr) {
            for (auto *const actor: current_scene->actors) {
                actor->update(delta_time);
            }
        }
    }

    void render() {
        Scene *current_scene = EngineSettingsSingleton::get_instance().get_current_scene();
        if (current_scene != nullptr) {
            for (auto *const actor: current_scene->actors) {
                actor->render();
            }
        }
    }

    void terminate() { CloseWindow(); }
} // namespace Engine
