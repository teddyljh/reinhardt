#include <iostream>
#include "rein.h"

using namespace std;

/*
 * Implementation file for class generating Reinhardt polynomials for n
 * according to "Sporadic Reinhardt polygons 2".  Employs a recursive
 * strategy in enumerating all possible Ai's and Bi's.
 * M. Mossinghoff 7/7/14
 */

Reinhardt::Reinhardt(int pp, int qq, int* rp, int rpn) : p(pp), q(qq), rn(rpn) {
    rcum = new int[rn+1];
    rcum[0] = 0;
    for (int i=0; i<rn; i++) rcum[i+1] = rcum[i] + rp[i];
    r = rcum[rn];
    n = p*q*r;
    f1 = new int[p*r];
    for (int i=0; i<p*r; i++) f1[i] = 0;
    f2 = new int[q*r+1];
    for (int i=0; i<q*r+1; i++) f2[i] = 0;
    F = new int[n];
}

Reinhardt::~Reinhardt() {
    delete [] rcum;
    delete [] f1;
    delete [] f2;
    delete [] F;
}

void Reinhardt::go() {
    //buildA(0, 1);
    //buildA(0, -1);
    f1[0] = 1;
    buildA(0, 0, 1, rcum[1], -1);
}

// Find all allowable patterns in the ith block of size r in f1.
// Start with sign s.
void Reinhardt::buildA(int i, int s) {
    //cout << "buildA(" << i << "," << s << ")" << endl;
    if (i == p) {
        // We are done with the A's so start on the B's.
        buildB(0, 1, rcum[1], rcum[2], s);
    } else {
        // Pick a position to set to s, then start recursion for rest.
        for (int k=i*r; k<=i*r+rcum[1]; k++) {
            f1[k] = s;
            buildA(i, 0, k+1, i*r+rcum[1], -s);
            f1[k] = 0;
        }
    }
}

// In the jth part of the ith r-block of A, set an even number of positions
// between positions start and stop with an alternating sign pattern, starting
// with -1.
void Reinhardt::buildA(int i, int j, int start, int stop, int s) {
    //cout << "buildA(" << i << "," << j << "," << start << "," << stop << "," << s << ")" << endl;
    if (j >= rn) {
        buildA(i+1, s);
    } else {
        buildA(i, j+2, i*r+rcum[j+2], i*r+rcum[j+3], s);
        for (int k=start; k<=stop-1; k++) {
            f1[k] = s;
            for (int l=k+1; l<=stop; l++) {
                f1[l] = -s;
                buildA(i, j, l+1, stop, s);
                f1[l] = 0;
            }
            f1[k] = 0;
        }
    }
}

// In the jth part of the ith r-block of B, set an even number of positions
// between positions start and stop with an alternating sign pattern, starting
// with -1.
void Reinhardt::buildB(int i, int j, int start, int stop, int s) {
    //cout << "buildB(" << i << "," << j << "," << start << "," << stop << "," << s << ")" << endl;
    if (i == q) {
        f2[0] = -f2[q*r];
        buildF();
        f2[0] = 0;
    } else if (j >= rn) {
        buildB(i+1, 1, (i+1)*r+rcum[1], (i+1)*r+rcum[2], -s);
    } else {
        buildB(i, j+2, i*r+rcum[j+2], i*r+rcum[j+3], s);
        for (int k=start; k<=stop-1; k++) {
            f2[k] = s;
            for (int l=k+1; l<=stop; l++) {
                f2[l] = -s;
                buildB(i, j, l+1, stop, s);
                f2[l] = 0;
            }
            f2[k] = 0;
        }
    }
}

void Reinhardt::buildF() {
    for (int k=0; k<p*r; k++) {
        for (int j=0, sign=1; j<q; j++,sign=-sign) {
            F[j*p*r+k] = sign*f1[k];
        }
    }
    for (int k=0; k<q*r; k++) {
        for (int j=0, sign=1; j<p; j++,sign=-sign) {
            F[j*q*r+k] += sign*f2[k];
        }
    }
    printF();
}

// Uncomment the block if you want to see the f1 and f2.
void Reinhardt::printF() const {
/*
    cout << "f1: ";
    for (int k=0; k<p*r; k++) {
        if (f1[k] == 1) cout << '+';
        else if (f1[k] == -1) cout << '-';
        else cout << f1[k];
    }
    cout << endl << "f2: ";
    for (int k=0; k<q*r+1; k++) {
        if (f2[k] == 1) cout << '+';
        else if (f2[k] == -1) cout << '-';
        else cout << f2[k];
    }
    cout << endl;
*/
    for (int k=0; k<n; k++) {
        if (F[k] == 1) cout << '+';
        else if (F[k] == -1) cout << '-';
        else {
            cout << F[k];
            //if (F[k] != 0) cerr << "E";
        }
    }
    cout << '\n';
}
