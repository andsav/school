#include <iostream>
#include <vector>
#include <climits>
#include <unordered_map>

#define INF INT_MAX

using namespace std;

struct Grid {
    int n;

    vector<int> jump; // Snakes and ladders

    Grid() {
        int s, l, a, b;

        cin >> n >> s >> l;

        // initialize jump array
        jump.push_back(0); // box 0
        for(int i=1; i <= n*n; ++i)
            jump.push_back(i);

        for(int i=0; i < s+l; ++i) {
            cin >> a >> b;
            jump[a] = b;
        }
    }

    int operator[](int i) {
        return jump[i];
    }
};

struct Solver {
    Grid& G;
    int last;

    unordered_map<int, int> steps_from;
    unordered_map<int, bool> reaching_from;

    Solver(Grid &g) : G(g) {
        last = G.n * G.n;
        for(int i = 1; i < last; ++i) {
            steps_from[i] = INF;
            reaching_from[i] = false;
        }
        reaching_from[last] = true;
        steps_from[last] = 0;
    }

    Solver* run() {
        for(int i = last-1; i > 0; --i) {
            int box = G[i];
            int step_max = 0;
            for(int b = box+1; b <= box+6; ++b) {
                int current = G[b];

                if(reaching_from[current])
                    reaching_from[box] = true;

                if(current >= last) {
                    step_max = max(step_max, 1);
                }
                else if(steps_from[current] != INF) {
                    step_max = max(step_max, steps_from[current]+1);
                }
                else {
                    step_max = INF;
                }
            }
            steps_from[box] = step_max;
        }

        return this;
    }

    void out() {
        if(!reaching_from[1])
            cout << "impossible" << endl;
        else if(steps_from[1] == INF)
            cout << "infinity" << endl;
        else
            cout << steps_from[1] << endl;
    }
};

int main() {
    Grid G;
    Solver S(G);

    S.run()->out();
}