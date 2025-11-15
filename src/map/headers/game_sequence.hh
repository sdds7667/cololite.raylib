//
// Created by Ion Plamadeala on 14/11/2025.
//

#ifndef COLOLITE_GAMESEQUENCE_HH
#define COLOLITE_GAMESEQUENCE_HH
#include <vector>

namespace Game {
    enum class GamePhase {
        DO_NOTHING,
        NEXT_PHASE,
        WAIT_FOR_TIME,
        FREE_BUILDING,
        FREE_ROAD,
    };

    namespace GamePhaseParameters {
        struct WaitForTime {
            float time_in_s;
        };
    }

    class GamePhaseParameterized {
        GamePhase phase;
        void *parameter;

    public:
        GamePhaseParameterized(GamePhase phase, void *parameter);

        [[nodiscard]] GamePhase get_phase() const;

        static GamePhaseParameterized for_no_parameter(GamePhase phase);

        static GamePhaseParameterized wait_for_time(float time_in_s);

        [[nodiscard]] GamePhaseParameters::WaitForTime *get_wait_for_time_parameter() const;

        void cleanup() const;


        ~GamePhaseParameterized();
    };

    class GameAction {
        std::vector<GamePhaseParameterized> phases;
        int repeat_count = 1;
        int index = 0;

    public:
        static GamePhaseParameterized next_phase;

        explicit GameAction(const GamePhaseParameterized &phase);

        GamePhaseParameterized *get_next_phase();

        [[nodiscard]] bool is_finished() const;

        int priority = 0;
    };

    class GameActionComparator {
    public:
        bool operator()(const GameAction &a, const GameAction &b) const;
    };

    using GameSequence = std::vector<GameAction>;
};

#endif //COLOLITE_GAMESEQUENCE_HH
