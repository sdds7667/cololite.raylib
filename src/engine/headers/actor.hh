//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ACTOR_HH
#define COLOLITE_ACTOR_HH
#include <raylib.h>

namespace Engine {
    class Actor {
    public:
        virtual ~Actor() = default;

        virtual void update(float deltaTime) = 0;

        virtual void render() const = 0;
    };

    class ClickableActor {
    public:
        virtual ~ClickableActor() = default;

        [[nodiscard]] virtual bool is_clicked(const Vector2 &mouse_position) const = 0;
    };
}

#endif //COLOLITE_ACTOR_HH
