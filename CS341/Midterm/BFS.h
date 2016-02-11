//
// Created by Andrei on 16-02-10.
//

#ifndef MIDTERM_BFS_H
#define MIDTERM_BFS_H

#include "inc.h"
#include <queue>
#include <vector>

struct BFS {

    adj graph;

    BFS(adj &g);

    vector<int> path(int s, int t);

};

#endif //MIDTERM_BFS_H
