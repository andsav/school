#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, j, a, b, c, start, end;
    vector<int> array;

    cin >> n;
    while(cin >> j) array.push_back(j);

    sort(array.begin(), array.end());

    for(unsigned int i = 0; i <= array.size() - 3; ++i) {
        a = array[i];
        start = i+1;
        end = array.size()-1;

        while(start < end) {
            b = array[start];
            c = array[end];

            if(a+b+c == n)
                goto output;

            else if(a+b+c > n)
                end--;

            else
                start++;
        }

    }

    output:
    cout << a << endl << b << endl << c << endl;
}

