#ifndef rein_h
#define rein_h

/*
 * Class for generating all Reinhardt polynomials for n given by the
 * construction of "Sporadic Reinhardt polygons 2".  Employs a recursive
 * strategy in enumerating all possible Ai's and Bi's.
 * M. Mossinghoff 7/7/14
 */

class Reinhardt {
  public:
    Reinhardt(int, int, int*, int);
    ~Reinhardt();
    void go();

  private:
    int p, q, r, rn, n;
    int *rcum, *F, *f1, *f2;
    void buildA(int, int);
    void buildA(int, int, int, int, int);
    void buildB(int, int, int, int, int);
    void buildF();
    void printF() const;
};

#endif
