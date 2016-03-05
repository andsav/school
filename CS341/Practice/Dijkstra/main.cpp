#include <iostream>
#include <queue>
#include <unordered_map>
#include <climits>

#define INF INT_MAX

using namespace std;

struct Comp {
    bool operator()(const pair<char, int> lhs, const pair<char, int> rhs) {
        return lhs.first > rhs.second;
    }
};

int main() {
    int V, E, j;
    unordered_map<char, unordered_map<char, int> > G; // weighed graph
    char s, t, u, v;
    priority_queue<pair<char, int>, vector<pair<char, int> >, Comp> Q;
    unordered_map<char, bool> visited;
    unordered_map<char, int> d;

    unordered_map<char, vector<char> > path;

    cin >> V >> E;
    cin >> s >> t;

    for(int i=0; i<E; ++i) {
        cin >> u >> v;
        cin >> j;
        G[u][v] = j;
        d[v] = d[u] = INF;
        path[v] = path[u] = vector<char>();
    }
    d[s] = 0;
    
    // Make priority queue
    for(auto n : d)
        Q.push(make_pair(n.first, n.second));

    while(!Q.empty()) {
        u = Q.top().first;
        Q.pop();
        
        // Need to check and keep track of visited nodes
        // since we can't update PQ's keys
        if(visited.find(u) != visited.end()) continue;
        visited[u] = true;

        // Reached destination
        if(u == t) break;

        for(auto n : G[u]) {
            if(d[n.first] > d[u] + n.second) {
                d[n.first] = d[u] + n.second;

                // Keep track of the path (maybe there is a better way)
                path[n.first] = path[u];
                path[n.first].push_back(u);
                
                // STL priority_queue makes it difficult to update keys
                // Instead we push the updated key as a new entry. 
                // Since the updated priority is smaller, we are guaranteed to pop it before the old value
                // Only drawback is we have to keep track of the visited nodes
                Q.push(make_pair(n.first, d[n.first]));
            }
        }
    }
    
    for(auto p : path[t]) cout << p << " ";
    cout << t << endl;
}


