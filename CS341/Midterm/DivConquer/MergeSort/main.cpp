#include <iostream>
#include <deque>

using namespace std;

deque<int> merge_sort(deque<int>& array);
deque<int> merge(deque<int>& left, deque<int>& right);

int main() {
    int i;
    deque<int> array; // Using deque for constant time push and pop front and back

    while(cin >> i) {
        array.push_back(i);
    }

    for(auto j : merge_sort(array)) {
        cout << j << endl;
    }

}

deque<int> merge_sort(deque<int>& array) {
    if(array.size() == 1)
        return array;

    /*
     * Divide
     */
    int middle = (array.size() + 1)/2;
    deque<int> left(array.begin(), array.begin() + middle);
    deque<int> right(array.begin() + middle, array.end());

    left = merge_sort(left);
    right = merge_sort(right);

    /*
     * Conquer
     */
    return merge(left, right);
}

deque<int> merge(deque<int>& left, deque<int>& right) {
    deque<int> ret;

    while(left.size() > 0 && right.size() > 0) {
        if(left[0] > right[0]) {
            ret.push_back(right[0]);
            right.pop_front();
        }
        else {
            ret.push_back(left[0]);
            left.pop_front();
        }
    }

    while(left.size() > 0) {
        ret.push_back(left[0]);
        left.pop_front();
    }

    while(right.size() > 0) {
        ret.push_back(right[0]);
        right.pop_front();
    }

    return ret;
}