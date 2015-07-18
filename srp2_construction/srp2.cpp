#include <iostream>
#include <cstdlib>
#include <cmath>
#include "rein.h"

using namespace std;

/*
 * Generate all Reinhardt polynomials for n given by the construction of
 * "Sporadic Reinhardt polygons 2".  Invoke with "srp2 p q r1 ... rk",
 * where p and q are distinct odd primes, r1, ..., rk are positive
 * integers, and k is even.  Here, r1, ..., rk is a composition of r, and
 * n = pqr.
 *
 * M. Mossinghoff 7/7/14
 */

int isprime(int);

int main(int argc, char** argv) {
    if (argc <= 4 || !(argc & 1)) {
        cerr << "Use " << argv[0] << " p q r1 ... rk with k even" << endl;
        exit(1);
    }
    int p = atoi(argv[1]);
    int q = atoi(argv[2]);
    if (p < 3 || q < 3 || !isprime(p) || !isprime(q) || p==q) {
        cerr << "Error: p and q must be distinct odd primes." << endl;
        exit(1);
    }
    int rn = argc - 3;
    int* rpart = new int[rn];
    for (int i=0; i<rn; i++) {
        rpart[i] = atoi(argv[i+3]);
        if (rpart[i] <= 0) {
            cerr << "Error: each part of r must be positive." << endl;
            exit(1);
        }
    }

    Reinhardt reinhardt(p, q, rpart, rn);
    reinhardt.go();

    delete [] rpart;
    return 0;
}

// Simple function to determine whether or not p is prime.
// Precondition: p > 1.
int isprime(int p) {
    int s = static_cast<int>(sqrt(static_cast<double>(p)));
    if (p == 2) return true;
    if (!(p&1)) return false;
    for (int k=3; k<=s; k++) {
        if (p%k == 0) return false;
    }
    return true;
}
