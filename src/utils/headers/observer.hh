//
// Created by Ion Plamadeala on 13/11/2025.
//

#ifndef COLOLITE_OBSERVER_HH
#define COLOLITE_OBSERVER_HH
#include <unordered_map>
#include <string>
#include <vector>

class Listener {
    int id;
    static int s_id;

public:
    Listener();

    virtual ~Listener() = default;

    virtual void operator()(std::unordered_map<std::string, void *> deltas);

    bool operator==(const Listener &other) const;

    int get_id();
};

class Observable {
    std::vector<Listener *> listeners;

public:
    void add_listener(Listener *listener);

    void remove_listener(const Listener *listener);

    void broadcast_change(const std::unordered_map<std::string, void *> &deltas);
};

#endif //COLOLITE_OBSERVER_HH
