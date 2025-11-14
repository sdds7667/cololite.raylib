
#pragma once
#include "engine_settings.hh"
#include "map.hh"

namespace Engine {
    enum class AnimationValue {
        CORNER_HIGHLIGHT_SIZE = 0
    };


    void render_map(const RenderSettings &render_settings, const RenderResources &render_resources,
                    const ColorScheme &color_scheme, const Map::Map &map);

    void main_loop();
} // namespace Engine
