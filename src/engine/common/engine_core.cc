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
        SetTargetFPS(get_engine_settings().get_render_settings().target_fps);
    }

    void update(const float delta_time) {
        if (Scene *current_scene = get_engine_settings().get_current_scene(); current_scene != nullptr) {
            current_scene->update_all(delta_time);
        }
    }

    void render() {
        BeginDrawing();
        ClearBackground(WHITE);
        if (const Scene *current_scene = get_engine_settings().get_current_scene(); current_scene != nullptr) {
            current_scene->render_all();
        }
        EndDrawing();
    }

    void terminate() { CloseWindow(); }
} // namespace Engine
