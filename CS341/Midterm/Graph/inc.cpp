#include "inc.h"

adj adj_from_input() {
    adj res;
    int n, m, u, v;

    cin >> n >> m;

    while(cin >> u) {
        cin >> v;
        res[u].push_back(v);
        res[v].push_back(u);
    }

    return res;
}