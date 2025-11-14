//
// Created by Ion Plamadeala on 13/11/2025.
//

#include "observer.hh"

int Listener::s_id = 0;

bool Listener::operator==(const Listener &other) const {
    return id == other.id;
}

void Observable::add_listener(Listener *listener) {
    listeners.push_back(listener);
}

void Observable::remove_listener(const Listener *listener) {
    listeners.erase(std::find(listeners.begin(), listeners.end(), listener));
}

void Observable::broadcast_change(const std::unordered_map<std::string, void *> &deltas) {
    for (auto *listener: listeners) {
        listener->operator()(deltas);
    }
}

