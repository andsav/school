#ifndef ROUTING_STATE_H
#define ROUTING_STATE_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <climits>

#define INF INT_MAX

struct Comp {
    bool operator()(const std::pair<unsigned int, int> lhs, const std::pair<unsigned int, int> rhs) {
        return lhs.first > rhs.second;
    }
};

typedef std::unordered_map<unsigned int, std::unordered_map<unsigned int, int> > Graph;
typedef std::priority_queue<std::pair<unsigned int, int>, std::vector<std::pair<unsigned int, int> >, Comp> PriQue;

struct State {
    Graph graph;
    PriQue queue;

    unsigned int this_id;

    State(unsigned int);

};

#endif //ROUTING_STATE_H
