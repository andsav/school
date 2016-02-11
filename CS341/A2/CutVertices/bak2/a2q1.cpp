#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

typedef unordered_map<int, vector<int> > graph;

struct SortPairs {
    bool operator() (pair<int, int> i, pair<int, int> j) {
        if(i.first == j.first)
            return i.second < j.second;

        return i.first < j.first;
    }
};

struct Cut {
    vector<int> cut_vertices;
    vector<pair<int, int> > cut_edges;

    unordered_map<int, bool> visited;
    unordered_map<int, int> num, low, parent;

    SortPairs sortFn;

    int count;

    graph OG; // Original Graph

    Cut() : count(0) { }

    Cut* run() {
        for(auto vv : OG) {
            if(!this->visited[vv.first])
                this->dfs(vv.first);
        }

        for(auto v : this->cut_vertices) {
            for(auto w : this->OG[v]) {
                if(this->num[w] == this->low[w]) {
                    this->cut_edges.push_back(make_pair(v, w));
                }
            }
        }

        return this;
    }

    void dfs(int v) {
        this->visited[v] = true;
        this->num[v] = this->low[v] = this->count++;
        for(auto w : this->OG[v]) {
            if(!this->visited[w]) {
                this->parent[w] = v;
                this->dfs(w);

                this->low[v] = min(this->low[v], this->low[w]);

                if (this->low[w] >= this->num[v]) {
                    this->cut_vertices.push_back(v);
                }
            }
            else if(w != this->parent[v]) {
                this->low[v] = min(this->low[v], this->num[w]);
            }
        }
    }

    void out() {
        sort(cut_vertices.begin(), cut_vertices.end());
        sort(cut_edges.begin(), cut_edges.end(), sortFn);

        cout << cut_vertices.size();
        for(auto i : cut_vertices) cout << " "<< i;
        cout << endl;

        cout << cut_edges.size();
        for(auto p : cut_edges) cout << " " << "(" << p.first << "," << p.second << ")";
        cout << endl;
    }

};

int main() {
    int n, m, i, j;
    cin >> n >> m;

    Cut c;

    while(cin >> i) {
        cin >> j;
        c.OG[i].push_back(j);
        c.visited[i] = c.visited[j] = false;
    }

    c.run()->out();
}

