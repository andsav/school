#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

unsigned int merge(vector<int> &integers, vector<int> &left, vector<int> &right);
unsigned int merge_and_count(vector<int> &integers);

int main() {
    stringstream ss;
    string s;
    int i;
    vector<int> integers;

    while(cin >> s) {
        ss.clear();
        ss.str(s);
        ss >> i;
        integers.push_back(i);
    }

    cout << merge_and_count(integers) << endl;
	
    return 0;
}

unsigned int merge(vector<int> &integers, vector<int> &left, vector<int> &right) {
    unsigned int i, j, count;
    i = 0;
    j = 0;
    count = 0;

    while(i < left.size() or j < right.size()) {
        if(i == left.size()) {
            integers[i+j] = right[j];
            j++;
        }
        else if(j == right.size()) {
            integers[i+j] = left[i];
            i++;
        }
        else if(left[i] <= right[j]) {
            integers[i+j] = left[i];
            i++;
        }
        else {
            integers[i+j] = right[j];
            count += left.size()- i;
            j++;
        }
    }

    return count;
}

unsigned int merge_and_count(vector<int> &integers) {
    if(integers.size() < 2) return 0;

    int middle = (integers.size() + 1)/2;
    vector<int> left(integers.begin(), integers.begin() + middle);
    vector<int> right(integers.begin() + middle, integers.end());

    return merge_and_count(left) + merge_and_count(right) + merge(integers, left, right);
}
