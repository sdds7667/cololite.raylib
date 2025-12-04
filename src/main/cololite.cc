
#include "engine_core.hh"
#include "engine_settings.hh"
#include "raylib.h"
#include "scene.hh"

auto main() -> int {
    Engine::initialize();
    Engine::Scene main_scene;
    Engine::EngineSettingsSingleton::get_instance().set_scene(&main_scene);

    while (true) {
        Engine::update(GetFrameTime());
        Engine::render();
    }
    Engine::terminate();
    return 0;
}
