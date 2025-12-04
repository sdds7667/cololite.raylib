//
// Created by Ion Plamadeala on 14/11/2025.
//

#ifndef COLOLITE_GAME_HH
#define COLOLITE_GAME_HH

struct House {
    int level = 1;
};


class Road {
};

class Roll {
    int roll;

public:
    explicit Roll(int roll);

    [[nodiscard]] int get_roll() const;
};


#endif //COLOLITE_GAME_HH
