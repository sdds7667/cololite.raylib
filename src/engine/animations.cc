//
// Created by Ion Plamadeala on 13/11/2025.
//

#include "animations.hh"

#include <algorithm>

namespace Engine {
    BaseAnimation::BaseAnimation(const float animation_speed_in_s, const float starting_value,
                                 const float ending_value
    ) : animation_speed_in_s(animation_speed_in_s),
        animation_value(0.0f), starting_value(starting_value),
        ending_value(ending_value), current_value(starting_value) {
    }

    BaseAnimation::~BaseAnimation() = default;


    void LinearAnimation::tick_animation(float delta_time) {
        animation_value = std::min(1.0f, animation_value + delta_time / animation_speed_in_s);
        current_value = animation_value * (ending_value - starting_value) + starting_value;
    }

    bool LinearAnimation::is_finished() const {
        return animation_value == 1.0;
    }

    void AnimationSequence::add_animation(BaseAnimation *animation) {
        animations.push_back(animation);
    }

    void AnimationSequence::remove_animation(const BaseAnimation *animation) {
        for (auto i = animations.begin(); i != animations.end(); ++i) {
            if (*i == animation) {
                animations.erase(i);
                return;
            }
        }
        throw new std::invalid_argument("Animation not found");
    }

    bool AnimationSequence::is_finished() const {
        for (const auto &animation: animations) {
            if (!animation->is_finished()) {
                return false;
            }
        }
        return true;
    }

    void AnimationSequence::tick_animation(float delta_time) {
        for (const auto &animation: animations) {
            if (animation->is_finished()) continue;
            animation->tick_animation(delta_time);
            break;
        }
    }


    void BaseAnimation::reset() {
        animation_value = 0.0f;
    }

    float BaseAnimation::get_current_value() const {
        return current_value;
    }

    LinearAnimation::LinearAnimation(float animation_speed_in_s, float starting_value,
                                     float ending_value) : BaseAnimation(
        animation_speed_in_s, starting_value, ending_value) {
    }
}
