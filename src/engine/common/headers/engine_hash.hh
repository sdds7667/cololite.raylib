//
// Created by Ion Plamadeala on 13/11/2025.
//

#pragma once

#include <engine.hh>

template<>
struct std::hash<Engine::AnimationValue> {
    size_t operator()(const Engine::AnimationValue &s) const noexcept {
        size_t seed = 0;
        seed ^= std::hash<int>{}(static_cast<int>(s)) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

