
#include "engine_core.hh"
#include "engine_settings.hh"
#include "raylib.h"
#include "scene.hh"

auto main() -> int {
    Engine::initialize();
    Engine::Scene main_scene;
    Engine::get_engine_settings().set_scene(&main_scene);

    while (!WindowShouldClose()) {
        Engine::update(GetFrameTime());
        Engine::render();
    }
    Engine::terminate();
    return 0;
}
