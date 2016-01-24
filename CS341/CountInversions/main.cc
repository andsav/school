#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int mergeSort(vector<int>& integers, int left, int right);

int main() {
    stringstream ss;
    string s;
    vector<int> integers;
    int i;

    while(cin >> s) {
        ss.clear();
        ss.str(s);
        ss >> i;
        integers.push_back(i);
    }

    cout << mergeSort(integers, 1, 1) << endl;

    for(i = 0; i < integers.size(); ++i) {
        cout << integers[i] << endl;
    }

	return 0;
}

int mergeSort(vector<int>& integers, int left, int right) {
    return 1;
}