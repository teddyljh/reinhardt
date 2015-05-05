#include <iostream>
#include <cassert>
#include <cstdlib>
#include "polygonSearch.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc  != 2) {
        cerr << "Error: use " << argv[0] << " n" << endl;
        exit(1);
    }
    int n = atoi(argv[1]);
    assert(n >= 3);
    PolygonSearch ps(n);
    cout << "(* Polygons with " << n << " Sides and Optimal Perimeter *)\n";
    cout << "nml[{\n";
    //cerr << "Timing for polygon search for n = " << n << "\n";
    ps.findPolygons();
    cout << "}];" << endl;
    cout << "(* Number of polygons: " << ps.getHits() << " *)\n";
    cout << "(* Number of aperiodic polygons: " << ps.getAperiodic()
         << " *)" << endl;
    return 0;
}
