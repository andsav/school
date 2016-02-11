#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct SortPairs {
    bool operator() (pair<int, int> i, pair<int, int> j) {
        if(i.first == j.first)
            return i.second < j.second;

        return i.first < j.first;
    }
};

struct Graph {
    unordered_map<int, vector<int> > adjacency;
    unordered_map<int, bool> visited;
    unordered_map<int, int> num, low, parent;

    int N, counter;

    SortPairs sortFn;

    unordered_set<int> cut_vertices;
    vector<pair<int, int> > cut_edges;

    Graph(int n) : N(n), counter(0) {
        for(int i = 1; i <= N; ++i) {
            visited[i] = false;
            parent[i] = -1;
        }
    }

    Graph* run() {
        for(int i =1; i <= N; ++i) {
            if(!visited[i]) this->visit(i);
        }
        return this;
    }

    void visit(int u) {
        visited[u] = true;
        low[u] = num[u] = ++counter;

        int children = 0;
        for(auto v : adjacency[u]) {
            if(!visited[v]) {
                children++;
                parent[v] = u;
                this->visit(v);

                low[u] = min(low[u], low[v]);
                if((parent[u] == -1 && children > 1) || (parent[u] != -1 && low[v] >= num[u]))
                    cut_vertices.insert(u);

                if(low[v] > num[u]) {
                    cut_edges.push_back(make_pair(u, v));
                }
            }
            else if(v != parent[u])
                low[u] = min(low[u], num[v]);
        }
    }

    void out() {
        vector<int> cv(cut_vertices.begin(), cut_vertices.end());
        sort(cv.begin(), cv.end());
        sort(cut_edges.begin(), cut_edges.end(), sortFn);

        if(cut_edges.size() != 0 && N != 2) {
            cout << cv.size();
            for(auto i : cv) cout << " "<< i;
            cout << endl;
        }
        else cout << 0 << endl;

        cout << cut_edges.size();
        pair<int, int> last = make_pair(-1, -1);
        for(auto p : cut_edges)  {
            if(p != last)
                cout << " " << "(" << p.first << "," << p.second << ")";
            last = p;
        }
        cout << endl;
    }
};

int main() {
    int n, m, u, v;
    cin >> n >> m;

    Graph g(n);

    while(cin >> u) {
        cin >> v;
        g.adjacency[u].push_back(v);
        g.adjacency[v].push_back(u);
    }

    g.run()->out();
}