#include <iostream>
#include <climits>

#define INF INT_MAX

using namespace std;

struct EggFloor {
    int** array;
    int n, k;

    EggFloor(int N, int K) : n(N), k(K) {
        array = new int*[n+1];
        for(int i = 0; i <=n; ++i) {
            array[i] = new int[k+1];
            array[i][1] = 1;
            array[i][0] = 0;
        }

        for(int j = 1; j <= k; ++j)
            array[1][j] = j;

        for(int i = 2; i <= n; ++i)
            for(int j = 2; j <= k; ++j)
                array[i][j] = INF;
    }

    ~EggFloor() {
        for(int i = 0; i <= n; ++i)
            delete[] array[i];
        delete[] array;
    }

    /**
     * This allows to set only the values we need in the memoization table
     */
    int access(int i, int j) {
        if(array[i][j] == INF)
            for(int x = 1; x <= j; ++x)
                array[i][j] = min(array[i][j], 1 + max(access(i-1, x-1), access(i, j - x)));

        return array[i][j];
    }
};

ostream& operator<<(ostream& os, EggFloor& ef) {
    os << ef.access(ef.n, ef.k);
    return os;
}


int main() {
    int n, k; // n eggs, k floors
    cin >> n >> k;

    EggFloor ef(n, k);

    cout << ef << endl;
}