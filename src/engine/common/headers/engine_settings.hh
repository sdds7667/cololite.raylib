//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ENGINE_SETTINGS_HH
#define COLOLITE_ENGINE_SETTINGS_HH
#include "../../../game/headers/map.hh"
#include "raylib.h"
#include "scene.hh"

namespace Engine {
    struct ColorScheme {
        Color grassGreen;
        Color warmWoodBrown;
        Color stoneGray;
        Color brickRed;
        Color wheatYellow;
        Color desertSand;
        Color mapBorder;
        Color circleBorder;
        Color color1;
        Color color2;
        Color color3;
        Color color4;
    };

    struct ResourceSprites {
        Texture2D bricks;
        Texture2D sheep;
        Texture2D wood;
        Texture2D cactus;
        Texture2D stone;
        Texture2D wheat;
        Texture2D random;
        Texture2D resource_outline;
        Texture2D resource_stack;
        Texture2D trade;

        ~ResourceSprites();
    };

    struct UISprites {
        Texture2D upgrade;
    };

    struct Sprites {
        Texture2D house;
        Texture2D town;
    };


    struct RenderResources {
        Font map_font;
        ResourceSprites resource_sprites;
        Sprites sprites;
        UISprites ui;
    };

    struct RenderSettings {
        int target_fps;
        float hex_size;
        float full_hex_size;
    };

    const Texture2D &get_texture_for_resource(const RenderResources &render_resources, const Map::Resource &resource);

    class EngineSettingsSingleton {
        EngineSettingsSingleton();
        ColorScheme m_color_scheme;
        RenderResources m_render_resources;
        RenderSettings m_render_settings;
        Scene *m_current_scene{nullptr};

    public:
        EngineSettingsSingleton(const EngineSettingsSingleton &) = delete;
        auto operator=(const EngineSettingsSingleton &) -> EngineSettingsSingleton & = delete;
        static auto get_instance() -> EngineSettingsSingleton &;
        void set_scene(Scene *scene);
        [[nodiscard]] auto get_current_scene() const -> Scene *;
        [[nodiscard]] auto get_color_scheme() const -> const ColorScheme &;
        [[nodiscard]] auto get_render_resources() const -> const RenderResources &;
        [[nodiscard]] auto get_render_settings() const -> const RenderSettings &;
    };
} // namespace Engine

#endif // COLOLITE_ENGINE_SETTINGS_HH
