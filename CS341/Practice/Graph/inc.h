#ifndef MIDTERM_INC_H
#define MIDTERM_INC_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <climits>

#define NIL -1
#define MAX INT_MAX

using namespace std;

typedef unordered_map<int, int> dict;
typedef unordered_map<int, vector<int> > adj;

adj adj_from_input();

#endif //MIDTERM_INC_H
