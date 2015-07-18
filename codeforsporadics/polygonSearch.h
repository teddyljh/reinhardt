#include <string>
using namespace std;

class PolygonSearch {
  public:
    PolygonSearch(int n);
    ~PolygonSearch();
    void findPolygons();
    int getHits() const { return hits; }
    int getAperiodic() const { return aperiodic; }

  private:
    long long* poly;
    int* b, * cyc, * rv, * sumrv;
    int deg, phi2n, hits, aperiodic, maxz;
    const int n;
    static const string cycfile;

    void findPolygonsAux(int, int, int);
    bool testPoly(int);
    void hit();
    int period(int) const;
    void printState(int) const;
};
