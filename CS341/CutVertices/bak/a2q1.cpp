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

SortPairs sortPairs;

struct Tree {
    graph og;
    vector<int> dfs_tree;
    unordered_map<int, bool> visited;
    unordered_map<int, int> num, low;
    vector<int> cut_vertices;
    vector<pair<int, int> > cut_edges;

    Tree(graph& g) : og(g) {
        for(auto edge : g) {
            this->addNode(edge.first);
            this->dfs(edge.second);
        }
    }

    void dfs(vector<int>& nodes) {
        for(auto v : nodes) {
            if(this->visited.find(v) == this->visited.end()) {
                this->addNode(v);
                this->dfs(og[v]);
            }
        }
    }

    void addNode(int n) {
        if(this->visited.find(n) == this->visited.end()) {
            this->dfs_tree.push_back(n);
            this->visited[n] = true;
            this->num[n] = dfs_tree.size();
        }
    }

    void computeLow(int v) {
        this->low[v] = this->num[v];

        for(auto w : og[v]) {
            if(this->num[w] > this->num[v]) {
                this->computeLow(w);
                if(this->low[w] >= this->num[v]) {
                    cut_vertices.push_back(v);
                }
                if(this->low[w] > this->num[v]) {
                    cut_edges.push_back(make_pair(v, w));
                }
                this->low[v] = min(this->low[v], this->low[w]);
            }
            else if(this->num[w] != this->num[v]-1) {
                this->low[v] = min(this->low[v], this->num[w]);
            }
        }
    }

    void run() {
        // Root node exception
        if(og[dfs_tree[0]].size() > 1)
            cut_vertices.push_back(dfs_tree[0]);

        for(auto v : dfs_tree) {
            this->computeLow(v);
        }

        sort(cut_vertices.begin(), cut_vertices.end());
        sort(cut_edges.begin(), cut_edges.end(), sortPairs);

        for(auto a : this->low) {
            cout << a.first << " " << a.second << endl;
        }
    }

    void out() {
        cout << cut_vertices.size() << " ";
        for(auto i : cut_vertices) cout << i << " ";
        cout << endl;

        cout << cut_edges.size() << " ";
        for(auto p : cut_edges) cout << "(" << p.first << "," << p.second << ")" << " ";
        cout << endl;
    }

};

int main() {
    int n, m, i, j;
    graph g;

    cin >> n >> m;

    while(cin >> i) {
        cin >> j;
        g[i].push_back(j);
    }

    Tree dfs(g);
    dfs.run();
    dfs.out();

    /*
    for(auto ii : dfs.dfs_tree) {
        cout << ii << endl;
    }
     */

}
