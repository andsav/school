#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    vector<int> row, F;

    // INPUT
    row.push_back(0);
    while(cin >> n)
        row.push_back(n);

    // PROCESS
    F.push_back(0);
    F.push_back(row[1]);
    for(int i=2; i<row.size(); ++i)
        F.push_back(max(row[i] + F[i-2], F[i-1])); // Either current coin picked or previous

    // OUT
    cout << F[row.size()-1] << endl;
}
