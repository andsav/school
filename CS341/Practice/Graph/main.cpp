#include <iostream>
#include "inc.h"
#include "BFS.h"

using namespace std;

int main() {
    adj g = adj_from_input();
    BFS b(g);

    for(auto i : b.path(1, 5)) {
        cout << i << endl;
    }
}
