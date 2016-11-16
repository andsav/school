#ifndef ROUTING_GRAPH_H
#define ROUTING_GRAPH_H

#import <iostream>
#include <queue>
#include <unordered_map>
#include <climits>

#define INF INT_MAX

using namespace std;

struct Comp {
    bool operator()(const pair<unsigned int, int> lhs, const pair<unsigned int, int> rhs) {
        return lhs.first > rhs.second;
    }
};

typedef unordered_map<unsigned int, unordered_map<unsigned int, int> > Graph;
typedef priority_queue<pair<unsigned int, int>, vector<pair<unsigned int, int> >, Comp> PriQue;

struct LinkState {
    Graph graph;
    PriQue queue;


};

#endif //ROUTING_GRAPH_H
