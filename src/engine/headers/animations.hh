//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ANIMATIONS_HH
#define COLOLITE_ANIMATIONS_HH
#include <utility>
#include <vector>

namespace Engine {
    class BaseAnimation;

    class BaseAnimation {
    protected:
        float animation_speed_in_s;
        float animation_value;
        float starting_value;
        float ending_value;
        float current_value;

    public:
        BaseAnimation(float animation_speed_in_s, float starting_value, float ending_value);

        virtual ~BaseAnimation();

        virtual void tick_animation(float delta_time) = 0;

        void reset();

        [[nodiscard]] virtual bool is_finished() const = 0;

        [[nodiscard]] float get_current_value() const;
    };

    class LinearAnimation : public BaseAnimation {
    public:
        LinearAnimation(float animation_speed_in_s, float starting_value, float ending_value);

        ~LinearAnimation() override = default;

        void tick_animation(float delta_time) override;

        [[nodiscard]] bool is_finished() const override;
    };


    template<typename T>
    class AnimationContainer {
        std::vector<std::pair<T, BaseAnimation *> > animations{};

    public:
        AnimationContainer() = default;

        BaseAnimation *get_animation_for(const T &key) const {
            for (const auto &[animation_key, animation]: animations) {
                if (animation_key == key) {
                    return animation;
                }
            }
            throw std::runtime_error("No animation found");
        };

        void add_animation_for(T key, BaseAnimation *animation) {
            animations.emplace_back(key, animation);
        }

        void remove_animation_for(const T &key) {
            for (auto it = animations.begin(); it != animations.end(); ++it) {
                if (it->first == key) {
                    delete it->second;
                    animations.erase(it);
                    break;
                }
            }
        }

        void tick_animations(float delta_time) {
            for (const auto &[_, animation]: animations) {
                animation->tick_animation(delta_time);
            }
        }

        void remove_animation(BaseAnimation *animation) {
            for (auto it = animations.begin(); it != animations.end(); ++it) {
                if (it->second == animation) {
                    delete it->second;
                    animations.erase(it);
                    break;
                }
            }
        }

        ~AnimationContainer() {
            for (auto it = animations.begin(); it != animations.end(); ++it) {
                delete it->second;
            }
        }
    };

    class AnimationSequence : public BaseAnimation {
        std::vector<BaseAnimation *> animations{};

    public:
        explicit AnimationSequence() : BaseAnimation(0.0f, 0.0f, 1.0f) {
        }

        void add_animation(BaseAnimation *animation);

        void remove_animation(const BaseAnimation *animation);

        [[nodiscard]] bool is_finished() const override;

        ~AnimationSequence() override = default;

        void tick_animation(float delta_time) override;
    };
}

#endif //COLOLITE_ANIMATIONS_HH
