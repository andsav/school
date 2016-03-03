#include <iostream>
#include <vector>
#include <climits>

#define INF INT_MAX

using namespace std;

struct Grid {
    int n, N;

    vector<int> jump; // Snakes and ladders

    Grid() {
        int s, l, a, b;

        cin >> n >> s >> l;

        N = n*n;

        // initialize jump array
        jump.push_back(0); // box 0
        for(int i=1; i <= N; ++i)
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
    Solver(Grid &g) : G(g) { }

    void run() {
        int last = G.N;

        vector<int> steps_from(G.N+1, INF);
        vector<bool> reaching_from(G.N+1, false);

        steps_from[0] = 0;
        steps_from[last] = 0;
        reaching_from[last] = true;

        for(int i = last-1; i > 0; --i) {
            int box = G[i];
            int step_max = 0;
            for(int b = box+1; b <= box+6; ++b) {
                int current = G[b];
                if(current >= last) {
                    step_max = max(step_max, 1);
                }
                else if(steps_from[current] != INF) {
                    step_max = max(step_max, steps_from[current]+1);
                }
                else {
                    step_max = INF;
                    break;
                }
            }
            for(int b = box+1; b <= box+6; ++b) {
                int current = G[b];
                if(reaching_from[current]) {
                    reaching_from[box] = true;
                    break;
                }
            }
            steps_from[box] = step_max;
        }

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

    S.run();
}