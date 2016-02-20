#include <iostream>
#include <math.h>
#include <cstdio>

using namespace std;

unsigned long length(long n) {
    unsigned long ret = 0;
    do {
        ++ret;
        n /= 10;
    } while(n);

    return ret;
}

long karatsuba(long a, long b) {
    long x1, x2, y1, y2;

    long a_len = length(a);
    long b_len = length(b);

    char a_split[a_len];
    sprintf(a_split, "%ld", a);

    char b_split[b_len];
    sprintf(b_split, "%ld", b);

    // Base
    if(a_len == 1 || b_len == 1) {
        return a * b;
    }

    long deg = floor(a_len / 2);

}


int main() {
    long a, b;
    cin >> a >> b;

}