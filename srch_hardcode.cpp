/*/////////////////////////////////*/
/* Michael's Code for generating sporadic Reinhardt Polygons, pqr method
/*//////////////////////////////*/

#include <iostream>
using namespace std;

int num = 0;
const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int r = 10; /*additional value, our n is of the form pqr */

const int n = p*q*r;
int a[n], b[n], c[n], f3[n]; /*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

/* function declaration, most important are set, testC, findB, enumA, and hasPeriod */
void set(int*, int, int, int, int);
void printVec(int*, int);
void printGaps(int*, int);
void printAns();
bool testC();
void findB(int, int);
void enumA(int);
bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);


/* runs the creation of the coefficient sequences*/
int main() {
    for (int i=0; i<n; i++) a[i] = b[i] = c[i] = f3[i] = 0;
    set(a, n, q, 0, 1);
    enumA(p);
    return 0;
}

void set(int* v, int n, int q, int offset, int val) {
    int m = n/q; /* integer division, why integer division? */
    for (int j=0; j<q; j+=2) v[m*j+offset] = val; /*basically setting every other coefficient in a to 1 or -1 */
    for (int j=1; j<q; j+=2) v[m*j+offset] = -val; 

}

void enumA(int i) { 
    //cout << "in enumA" << endl;        /*weird nested loop structure; why is it like this? */
    if (i == 0) {
        findB(1, (n/p)/2);
    } else {
        set(a, n, q, i, 1);
        enumA(i-1);
        set(a, n, q, i, 0);
        enumA(i-1);
        set(a, n, q, i, -1);
        enumA(i-1);
        int k;
    }
}

// prior is the last nonzero term in c.
// k is the current index we need to set.
void findB(int prior, int k) {
    //cout << "in findB" << endl;
    if (k == n/p) {
        //cout << "printing" << endl;
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
    //HARDCODING IN f_3(x) = 1-z for n=210
    f3[0]= 1;
    //for (int j=0; j<n; j++) { cout << f3[j] << " ";}
    //    cout << endl;

    f3[1] = -1;
    
    f3[42] = -1;
    f3[43] = 1;
    f3[84] = 1;
    f3[85] = -1;
    f3[126] = -1;
    f3[127] = 1;
    f3[168] = 1;
    f3[169] = -1;

    for (int i=0; i<n; i++) c[i] = a[i] + b[i] + f3[n];
    int prior = 1;

    for (int i=1; i<n; i++) {
        if (c[i]) { //if random weird cases happen, don't even run isPeriodic and return
            if (c[i] == prior || c[i] > 1 || c[i] < -1) return false;
            prior = c[i];
        }
    }
    return !isPeriodic(c, n);
}


/****************************************************************/
/* printAns, printGaps and printVec are simply print formatting functions */
void printAns() {
cout << num << " " << "a:" << endl;
    printVec(a, n);
    cout << endl;
    printVec(b, n);
    cout << endl;
    printVec(f3,n);
    cout << endl;
    printVec(c, n);
    cout << endl;
    printGaps(c, n);
    cout << '\n' << endl;
    num++;
}


void printGaps(int* v, int n) { /*gives us the actual "list sequence" of numbers for the Reinhardt angle differences*/
    int k = 0, gap;
    while (++k < n) {
        gap = 0;
        for (gap=0; k < n && v[k] == 0; k++) gap++;
        cout << " " << (gap+1);
    }
}

void printVec(int* v, int n) { /*gives us our valid sequence of plus and minuses to print out*/
    for (int i=0; i<n; i++) {
        if (v[i] == 1) cout << '+';
        else if (v[i] == -1) cout << '-';
        else cout << v[i];
    }
}
/*************************************************************/
/* returns relevant properties about the  model, once it has been generated */

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
