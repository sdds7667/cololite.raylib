
#include "engine_settings.hh"
#include "raylib.h"

namespace Engine {
    EngineSettingsSingleton::EngineSettingsSingleton() {
        // Initialize color scheme
        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        m_color_scheme = {.grassGreen = {83, 130, 75, 255},
                          .warmWoodBrown = {136, 84, 24, 255},
                          .stoneGray = {121, 121, 121, 255},
                          .brickRed = {174, 32, 36, 255},
                          .wheatYellow = {244, 201, 95, 255},
                          .desertSand = {225, 169, 95, 255},
                          // .mapBorder = {120, 120, 120, 255},
                          // .circleBorder = {125, 125, 125, 255},
                          .mapBorder = {30, 30, 30, 255},
                          .circleBorder = {30, 30, 30, 255},
                          .color1 = {0, 112, 110, 255},
                          .color2 = {255, 99, 71, 255},
                          .color3 = {98, 44, 168, 255},
                          .color4 = {218, 165, 32, 255}};
        // Load render resources (textures, fonts, etc.)
        // Note: Actual texture loading code is omitted for brevity

        // Set render settings
        m_render_settings.hex_size = 50.0F;
        m_render_settings.full_hex_size = m_render_settings.hex_size * 2.0F;
        m_render_resources = {
                .map_font = LoadFontEx("resources/OpenSans-Regular.ttf", 128, nullptr, 250),
                .resource_sprites =
                        {
                                .bricks = LoadTexture("resources/sprites/resources/resource_bricks.png"),
                                .sheep = LoadTexture("resources/sprites/resources/resource_sheep.png"),
                                .wood = LoadTexture("resources/sprites/resources/resource_wood.png"),
                                .cactus = LoadTexture("resources/sprites/resources/resource_cactus.png"),
                                .stone = LoadTexture("resources/sprites/resources/resource_stone.png"),
                                .wheat = LoadTexture("resources/sprites/resources/resource_wheat.png"),
                                .random = LoadTexture("resources/sprites/resources/resource_random.png"),
                                .resource_outline = LoadTexture("resources/sprites/resources/resource_outline.png"),
                                .resource_stack = LoadTexture("resources/sprites/resources/coin_stack.png"),
                                .trade = LoadTexture("resources/sprites/resources/trade.png"),
                        },
                .sprites =
                        {
                                .house = LoadTexture("resources/sprites/house.png"),
                                .town = LoadTexture("resources/sprites/village.png"),
                        },
                .ui = {
                        .upgrade = LoadTexture("resources/sprites/ui/upgrade.png"),
                }};
        m_render_settings.target_fps = GetMonitorRefreshRate(0);
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    }

    void EngineSettingsSingleton::set_scene(Scene *scene) { m_current_scene = scene; }
    auto EngineSettingsSingleton::get_current_scene() const -> Scene * { return m_current_scene; }

    auto EngineSettingsSingleton::get_instance() -> EngineSettingsSingleton & {
        static EngineSettingsSingleton instance;
        return instance;
    }

    auto EngineSettingsSingleton::get_color_scheme() const -> const ColorScheme & { return m_color_scheme; }
    auto EngineSettingsSingleton::get_render_resources() const -> const RenderResources & { return m_render_resources; }
    auto EngineSettingsSingleton::get_render_settings() const -> const RenderSettings & { return m_render_settings; }
} // namespace Engine
