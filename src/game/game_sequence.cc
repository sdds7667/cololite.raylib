//
// Created by Ion Plamadeala on 14/11/2025.
//

#include "headers/game_sequence.hh"


namespace Game {
    bool GameActionComparator::operator()(const GameAction &a, const GameAction &b) const {
        return a.priority < b.priority;
    }

    bool GameAction::is_finished() const {
        return repeat_count == 0;
    }

    GamePhaseParameterized::GamePhaseParameterized(GamePhase phase,
                                                   void *parameter) : phase(phase),
                                                                      parameter(parameter) {
    }

    GamePhase GamePhaseParameterized::get_phase() const {
        return phase;
    }


    GamePhaseParameterized GamePhaseParameterized::for_no_parameter(GamePhase phase) {
        if (phase == GamePhase::WAIT_FOR_TIME) {
            throw new std::invalid_argument(
                "WAIT_FOR_TIME phase requires parameter. Use the specific method to create it");
        }
        return GamePhaseParameterized(phase, nullptr);
    }

    GamePhaseParameterized GamePhaseParameterized::wait_for_time(float time_in_s) {
        auto parameter = new GamePhaseParameters::WaitForTime{time_in_s};
        return GamePhaseParameterized(GamePhase::WAIT_FOR_TIME, parameter);
    }

    GamePhaseParameters::WaitForTime *GamePhaseParameterized::get_wait_for_time_parameter() const {
        if (phase != GamePhase::WAIT_FOR_TIME) {
            throw std::runtime_error("This phase does not have a wait_for_time parameter");
        }
        return reinterpret_cast<GamePhaseParameters::WaitForTime *>(parameter);
    }

    void GamePhaseParameterized::cleanup() const {
        if (parameter != nullptr) {
            switch (phase) {
                case GamePhase::WAIT_FOR_TIME:
                    delete reinterpret_cast<GamePhaseParameters::WaitForTime *>(parameter);
                default:
                    break;
            }
        }
    }

    GamePhaseParameterized::~GamePhaseParameterized() = default;


    GameAction::GameAction(const GamePhaseParameterized &phase) {
        phases.push_back(phase);
    }

    GamePhaseParameterized *GameAction::get_next_phase() {
        GamePhaseParameterized *result = nullptr;
        if (!is_finished()) {
            result = &phases[index++];
            if (index == phases.size()) {
                index = 0;
                repeat_count--;
            }
        } else {
            result = &next_phase;
        }
        return result;
    }

    GamePhaseParameterized GameAction::next_phase = GamePhaseParameterized::for_no_parameter(
        GamePhase::NEXT_PHASE);
}

