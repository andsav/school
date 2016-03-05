#include <iostream>

using namespace std;

int main() {
    int i, ret, end;
    ret = end = 0;

    while(cin >> i) {
        end = max(end + i, 0);

        if(end > 0)
            ret = end;
    }

    cout << ret << endl;

}