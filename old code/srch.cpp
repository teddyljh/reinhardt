#include <iostream>
using namespace std;

const int p = 3;
const int q = 5;
const int r = 4;
const int n = p*q*r;
int a[n], b[n], c[n];

void set(int*, int, int, int, int);
void printVec(int*, int);
void printGaps(int*, int);
void printAns();
bool testC();
void findB(int, int);
void enumA(int);
bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);

int main() {
    for (int i=0; i<n; i++) a[i] = b[i] = c[i] = 0;
    set(a, n, q, 0, 1);
    enumA(q);
    return 0;
}

void set(int* v, int n, int q, int offset, int val) {
    int m = n/q;
    for (int j=0; j<q; j+=2) v[m*j+offset] = val;
    for (int j=1; j<q; j+=2) v[m*j+offset] = -val;
}

void enumA(int i) {
    if (i == 0) {
        findB(1, 1);
    } else {
        set(a, n, q, i, 1);
        enumA(i-1);
        set(a, n, q, i, 0);
        enumA(i-1);
        set(a, n, q, i, -1);
        enumA(i-1);
    }
}

// prior is the last nonzero term in c.
// k is the current index we need to set.
void findB(int prior, int k) {
    if (k == n/p) {
        if (testC()) printAns();
    } else if (a[k] == prior) {
        set(b, n, p, k, -prior);
        findB(prior, k+1);
    } else if (a[k] == -prior) {
        set(b, n, p, k, 0);
        findB(a[k], k+1);
        set(b, n, p, k, prior);
        findB(prior, k+1);
    } else {  // a[k] == 0
        set(b, n, p, k, 0);
        findB(prior, k+1);
        set(b, n, p, k, -prior);
        findB(-prior, k+1);
    }
}

bool testC() {
    for (int i=0; i<n; i++) c[i] = a[i] + b[i];
    int prior = 1;
    for (int i=1; i<n; i++) {
        if (c[i]) {
            if (c[i] == prior || c[i] > 1 || c[i] < -1) return false;
            prior = c[i];
        }
    }
    return !isPeriodic(c, n);
}

void printAns() {
cout << "a: ";
    printVec(a, n);
    cout << endl;
    printVec(b, n);
    cout << endl;
    printVec(c, n);
    cout << endl;
    printGaps(c, n);
    cout << '\n' << endl;
}

void printGaps(int* v, int n) {
    int k = 0, gap;
    while (++k < n) {
        gap = 0;
        for (gap=0; k < n && v[k] == 0; k++) gap++;
        cout << " " << (gap+1);
    }
}

void printVec(int* v, int n) {
    for (int i=0; i<n; i++) {
        if (v[i] == 1) cout << '+';
        else if (v[i] == -1) cout << '-';
        else cout << v[i];
    }
}

bool isPeriodic(int* c, int n) {
    for (int t=2; t<=n/2; t++) {
        if (hasPeriod(c, n, t)) return true;
    }
    return false;
}

bool hasPeriod(int* c, int n, int t) {
    for (int i=0; i<n-t; i++) {
        if (c[i] != -c[i+t]) return false;
    }
    return true;
}
