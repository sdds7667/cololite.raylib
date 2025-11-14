//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_ACTOR_HH
#define COLOLITE_ACTOR_HH

namespace Engine {
    class Actor {
    public:
        virtual ~Actor() = default;

        virtual void update(float deltaTime) = 0;

        virtual void render() const = 0;
    };
}

#endif //COLOLITE_ACTOR_HH
