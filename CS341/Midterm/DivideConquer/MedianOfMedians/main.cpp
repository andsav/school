#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

vector<int> A;


int partition5(int left, int right);
int partition(int left, int right, int pivot);

int pivot(int left, int right);
int select(int left, int right, int n);

int main() {
    int i;
    while(cin >> i) A.push_back(i);


}

/*
 * Find median of 5 numbers
 */
int partition5(int left, int right) {
    assert(right - left == 4);

    vector<int> a(&A[left], &A[right]);
    if(a[1] < a[0])
        iter_swap(a.begin(), a.begin()+1);
    if(a[2] < a[3])
        iter_swap(a.begin()+2, a.begin()+3);

    if(a[2] < a[0]) {
        iter_swap(a.begin()+1, a.begin()+4);
        a[2] = a[0];
    }

    a[0] = a[4];
    if(a[0] < a[2]) {
        iter_swap(a.begin()+1, a.begin()+4);
        a[0] = a[2];
    }

    return min(a[3], a[0]);
}

int pivot(int left, int right) {
    if(right-left == 4)
        return partition5(left, right);

    for(int i = left; i < right; i += 5) {
        int r = max(i+4, right);
        int m5 = partition5(i, r);

    }
}