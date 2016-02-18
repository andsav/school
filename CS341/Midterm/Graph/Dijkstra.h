#ifndef MIDTERM_DJIKSTRA_H
#define MIDTERM_DJIKSTRA_H

#include "inc.h"
#include <queue>
#include <vector>

struct Dijkstra {

    adj_weighted graph;

    Dijkstra(adj_weighted &g);

    vector<int> path(int s, int t);

};

#endif //MIDTERM_DJIKSTRA_H
