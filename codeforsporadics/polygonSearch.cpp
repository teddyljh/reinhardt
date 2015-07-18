///////////////////////////////////////////
/* polygonSearch.cpp

class implementation for iterating through every possible
polygon wth n = 2pq sides.


Michael Mossinghoff
*/
///////////////////////////////////////////


#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "polygonSearch.h"

using namespace std;

PolygonSearch::PolygonSearch(int nn) : n(nn) {
    poly = new long long[n];   // Coeffs of polynomial.
    rv = new int[n];  // For Reuleaux vector
    sumrv = new int[n];  // Exponents in the polynomial, omitting 0.
    for (int i=0; i<n; i++) poly[i] = 0LL;

    ifstream in;
    in.open(cycfile.c_str());
    if (in.fail()) {
        cerr << "Error opening the file with the cyclotomic data, "
             << cycfile << ".\n";
        exit(1);
    }
    // Assume cycfile is sequential in n, starting with n=1.
    string junk;
    for (int k=1; k<n; k++) getline(in, junk);
    in >> phi2n >> phi2n;
    cyc = new int[phi2n + 1];    // Coeffs of Phi_2n(x)
    for (int i=0; i<=phi2n; i++) {
        in >> cyc[i];
        poly[i] = cyc[i];
    }
    in.close();
    b = new int[n - phi2n];  // Coeffs of multiplier, g(x)
    b[0] = 1;
    hits = 0;
    aperiodic = 0;
    maxz = n;  // Maximal number of consecutive zero coeffs allowed in poly.
    #ifdef debug
        cout << "n = " << n << "\n"
             << "phi(2n) = " << phi2n << "\n"
             << "Phi_2n(x): " << cyc[0];
        for (int i=1; i<=phi2n; i++) cout << " " << cyc[i];
        cout << "\n";
    #endif
}

PolygonSearch::~PolygonSearch() {
    delete [] poly;
    delete [] b;
    delete [] cyc;
    delete [] rv;
    delete [] sumrv;
}

void PolygonSearch::findPolygons() {
    if (testPoly(0)) hit();
    findPolygonsAux(1, -1, 0);
}

void PolygonSearch::findPolygonsAux(int m, int s, int curz) {
    #ifdef debug
        cout << "Entering m = " << m << ", s = " << s << "\n";
    #endif
    if (m == n-phi2n) return;
    b[m] = -poly[m];
    poly[m] = 0;
    if (b[m]) {
        for (int i=1; i<=phi2n; i++) poly[m+i] += b[m]*cyc[i];
        #ifdef debug
            cout << " poly[" << m << "] was set to 0.\n";
            cout << " b[" << m << "] = " << b[m] << ".\n";
            printState(m);
        #endif
        if (testPoly(m)) hit();
    }
    if (curz < maxz) findPolygonsAux(m+1, s, curz+1);
    if (s == 1) {
        b[m]++;
        poly[m] = 1LL;
        for (int i=1; i<=phi2n; i++) poly[m+i] += cyc[i];
        #ifdef debug
            printState(m);
        #endif
        if (testPoly(m)) hit();
    } else {
        b[m]--;
        poly[m] = -1LL;
        for (int i=1; i<=phi2n; i++) poly[m+i] -= cyc[i];
        #ifdef debug
            printState(m);
        #endif
        if (testPoly(m)) hit();
        if (maxz >= m) maxz = m-1;
    }
    findPolygonsAux(m+1, -s, 0);
    for (int i=0; i<=phi2n; i++) poly[m+i] -= b[m]*cyc[i];
}

// Check poly[1]..poly[m+phi2n] for pattern [0*-0*+]*0*
bool PolygonSearch::testPoly(int m) {
    for (deg=m+phi2n; !poly[deg]; deg--);  // Find last nonzero term.
    #ifdef debug
        cout << " Testing poly list:";
        for (int i=0; i<=m+phi2n; i++) cout << " " << poly[i];
        cout << "\n";
        cout << "deg is " << deg << "\n";
    #endif
    if (poly[deg] != 1) return false;
    for (int k=1; k<=deg; k++) {
        while (!poly[k]) k++;  // We know poly[deg] is 1.
        if (poly[k++] != -1) return false;
        while (!poly[k]) k++;
        if (poly[k] != 1) return false;
    }
    return true;
}

void PolygonSearch::hit() {
    int i, k, last, r = 1, t;
    for (k=1; !poly[k]; k++);
    last = sumrv[0] = rv[0] = k++;
    while (k <= deg) {
        while (!poly[k]) k++;
        rv[r] = k - last;
        sumrv[r] = k;
        if (rv[r] > rv[0]) return;  // Ensure rv[0] is largest in list.
        last = k++;
        r++;
    }
    rv[r] = n - deg;
    sumrv[r] = n;

    // For each occurrence of rv[0] in the rv array, say one beginning at
    // position k, check that the forward sum from rv[0] lexically exceeds
    // both the forward and backward sums from rv[k].  First check that the
    // forward sum from rv[0] lexically exceeds the backward sum from
    // rv[0].
    int sum = rv[0];                    //DIHEDRAL CHECK SITUATION
    for (i=1; i<=r; i++) {              //if in normal position already, they keep it
        sum += rv[r+1-i];               //does NOT move them into normal position
        if (sum > sumrv[i]) return;     //WHAT I WANT TO DO IS MOVE IT INTO NORMAL POSITION
        if (sum < sumrv[i]) break;
    }

    for (k=1; k<=r; k++) {
        if (rv[k] == rv[0]) {
            // forward scan
            sum = rv[k];
            for (i=1; i<=r; i++) {
                sum += rv[(k+i)%(r+1)];
                if (sum > sumrv[i]) return;
                if (sum < sumrv[i]) break;
            }
            // backward scan
            sum = rv[k];
            for (i=1; i<=r; i++) {
                sum += rv[(r+1+k-i)%(r+1)];
                if (sum > sumrv[i]) return;
                if (sum < sumrv[i]) break;
            }
        }
    }

/*
    // Find first number in rv not matching rv[0].  Call this index t.
    for (t=1; t<=r && rv[t]==rv[0]; t++);
    if (t <= r) {  // if t>r then all items identical, so keep it.
        // Scan rv from pos t+1 to r-1.  Check that any element adjacent to
        // a value equalling rv[0], but not itself equal to rv[0], is <= rv[t].
        for (k=t+1; k<r; k++) {
            if (rv[k] == rv[0]) {
                if (rv[k+1] < rv[0] && rv[k+1] > rv[t]) return;
                if (rv[k-1] < rv[0] && rv[k-1] > rv[t]) return;
            }
        }
        // More extensive check for rv[r], since it is adjacent to rv[0].
        // Stronger check: ensure rv[r] <= rv[t], but also if these are
        // equal then check rv[r-k] vs. rv[t+k] until finding a mismatch,
        // if any.
        for (k=0; (k<<1)<r-t && rv[r-k]==rv[t+k]; k++);
        if ((k<<1)<r-t && rv[r-k] > rv[t+k]) return;
    }
*/
    if (period(r) == r) {
        cout << "{" << rv[0] << ",";
        for (int i=1; i<r; i++) cout << rv[i] << "," ;
        cout << rv[r] << "},\n";
        aperiodic++;
    }
    //{ cout << "(* Aperiodic! *)" << endl; }
//cout << "(* period " << p << " *)" << endl;
    hits++;
}

int PolygonSearch::period(int r) const {
    for (int d=1; d<=(r+1)/2; d++) {
        bool same = true;
        for (int k=0; same && k<=r-d; k++) {
            if (rv[k] != rv[k+d]) same = false;
        }
        if (same) return d;
    }
    return r;
}

void PolygonSearch::printState(int m) const {
    cout << " g(x): " << b[0];
    for (int i=1; i<=m; i++) {
        if (b[i] == 1) cout << "+x^" << i;
        else if (b[i] == -1) cout << "-x^" << i;
        else if (b[i] > 0) cout << "+" << b[i] << "*x^" << i;
        else if (b[i] < 0) cout << "-" << -b[i] << "*x^" << i;
    }
    cout << "\n";
    cout << " f(x): " << poly[0];
    for (int i=1; i<=m+phi2n; i++) {
        if (poly[i] == 1) cout << "+x^" << i;
        else if (poly[i] == -1) cout << "-x^" << i;
        else if (poly[i] > 0) cout << "+" << poly[i] << "*x^" << i;
        else if (poly[i] < 0) cout << "-" << -poly[i] << "*x^" << i;
    }
    cout << "\n";
}

const string PolygonSearch::cycfile = "cycdata.txt";
