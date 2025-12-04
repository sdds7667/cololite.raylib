
#pragma once
#include <vector>

#include "generic_actors.hh"

namespace Engine {
    struct Scene {
        std::vector<IActor *> actors;
    };
}
