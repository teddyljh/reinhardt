#include <iostream>
using namespace std;

int num = 0;
const int p = 3; /*prime number 1 */
const int q = 5; /*prime number 2 */
const int r = 2; /*additional value, our n is of the form pqr */
const int n = p*q*r;
int a[n], b[n], c[n], f3[n]; /*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

/* function declaration, most important are set, testC, findB, enumA, and hasPeriod */
void set(int*, int, int, int, int);
void enumA(int ); 


int main() {
    for (int i=0; i<n; i++) a[i] = b[i] = c[i] = f3[i] = 0;
    set(a, n, q, 0, 1);
    enumA(q);
    return 0;
}

void set(int* v, int n, int q, int offset, int val) {
    int m = n/q; /* integer division, why integer division? */
    for (int j=0; j<q; j+=2) v[m*j+offset] = val; /*basically setting every other coefficient in a to 1 or -1 */
    for (int j=1; j<q; j+=2) v[m*j+offset] = -val; 

}

void enumA(int i) { 
    //cout << "in enumA" << endl;        /*weird nested loop structure; why is it like this? */
        for (int j=0; j<n; j++) {
            cout << a[j] << " ";
        }
        set(a, n, q, i, 1);
        enumA(i-1);
        set(a, n, q, i, 0);
        enumA(i-1);
        set(a, n, q, i, -1);
        enumA(i-1);
       
    
}