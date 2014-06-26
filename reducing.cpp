#include <iostream>
using namespace std;

int num = 0;
const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int l = 5;
const int r = 1; /*additional value, our n is of the form pqr */

const int n = p*l*q*r;
int g1[n], g2[n], g3[n], g4[n], f2[14], f3[20], f1[34],  F[n];
bool boolc[n][3];
/*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

void printVec(int*, int);
void printGaps(int*, int);
void printAns();
bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);
void setCoeffs();
void multiply(int*, int*, int*);
bool determineBool(int* , int*);
void repeat(int*, int* , int, int);
bool checkAlt(int*);
void combine(int* , int* , int*);
void finish(int);

////////////////////////////////////////////////////////////////////////
/* setCoeffs - basic setup for all coefficient vectors use within */
void setCoeffs() {
	for (int x=0; x < n; x++) {
		for (int y=0; y <n ; y++ ){
			boolc[x][y] = false;
		}
	}
	for (int i=0; i<n; i++) g1[i] = g2[i] = g3[i] = F[i] = 0;
	for (int e=0; e<14; e++) f2[e] = 1;
	for (int f=0; f<20; f++) f3[f] = 1;
	for (int h=0; h<34; h++) f1[h] = 0;
}
///////////////////////////////////////////////////////////////////////////
/* repeat - repeats the coefficient sequence of v, with alternating sign,
and sets them to be the coefficient sequence of w.
	recursive definition
*/
void repeat(int* v, int* w, int num, int rep) {
	int size1 = sizeof(v)/sizeof(int); 
	int size2 = sizeof(w)/sizeof(int); //WATCH OUT; PRINT OUT, could give wrong results

	if (rep == size2) {
		return;
	}

	for (int i = rep; i < size2/size1; i++ ) {
		w[i] = num*v[i];

	}
	repeat(v, w, -num, rep+size1);

}

////////////////////////////////////////////////////////////////////////////
/* determineBool - determines all possible coefficient values for g3[n]
based on the fact that F[n] can only have coefficient values in -1, 1, or 0
*/
bool determineBool(int* v, int* w) {
	
	for (int i=0; i< n; i++) {
		if (v[i] == w[i] == 1) {
			boolc[i][0] = true; 
			boolc[i][1] = boolc[i][2] = false;
		}

		else if ((v[i] == 0 && w[i] == 1) || (v[i] == 1 && w[i] == 0)) {
			boolc[i][0] = boolc[i][1] = true;
			boolc[i][2] = false;
		}

		else if ((v[i] == -1 && w[i] == 0) || (v[i] == 0 && w[i] == -1)) {
			boolc[i][0] = false;
			boolc[i][1] = boolc[i][2] = true;
		}

		else if ((v[i] == w[i] == -1)) {
			boolc[i][0] = boolc[i][1] = false;
			boolc[i][2] = true;
		}

		else if ((v[i] == w[i] == 0) || (v[i] == -1 && w[i] == 1) || (v[i] == 1 && w[i] == -1)) {
			boolc[i][0] = boolc[i][1] = boolc[i][2] = true;
		}

	}
}

/////////////////////////////////////////////////////////////////////////
/* checkAlt - checks the necessary condition for F[n] that (a) it has an alternating sequence of -/+'s
and (b) that it has an odd number [reduces to checking whether or not the last sign is +] */
bool checkAlt(int* v) {
	if (v[0] != 1) return false;
	int sign = -1;
	
	for (int i=1; i<n; i++) {
		if (v[i] == 0) continue;
		if (v[i] == sign) return false;
		if (v[i] == -sign) {
			sign = -sign;
		}
	}
	if (sign == -1) {
		return false;
	} else {
	return true && (!isPeriodic(v, n));
	}
}
//////////////////////////////////////////////////////////////////////////
/* combine - adds three coefficient vectors to determine the final Reinhardt polynomial */
void combine(int* v, int* w, int *x) {
	for (int i=0; i<n; i++) {
		F[i] = v[i] + w[i] + x[i];
	}
}

//////////////////////////////////////////////////////////////
/* finish - calculates all possible g1's from the boolean array, checks their
/* validity as Reinhardt polynomials, and prints out any valid results */
void finish(int start) {
	//want the results to be equal to g1
	if (start == n+1) {
		combine(g1,g2,g3);
		if (checkAlt(F)) printAns();
	}

	if (boolc[start][0]) {
		g1[start] = -1;
		finish(start+1);
	}	

	if (boolc[start][1]) {
		g1[start] = 0;
		finish(start+1);
	}

	if (boolc[start][2]) {
		g1[start] = 0;
		finish(start+1);
	}

}

///////////////////////////////////////////////////////////////
int main() {
	
	cout << "START EXECUTION" << endl;
	//sets up the coefficient vectors
	setCoeffs();
	int maxf2 = 14;
	int maxf3 = 20;
	
	while (true) {

		while(true) {
	        repeat(f2, g2, 1, 0);
			repeat(f3, g3, 1, 0);

			determineBool(g2, g3);
			finish(0);

	       	// next iteration:
	       	f3[maxf3]--;
	       	for(int j= maxf3; j>1; j--) {  //decreasing
	       	   if (f3[j] < -1) {
	           	f3[j-1]--;
	     	   	f3[j]=1;
	        	}

	        	if(f3[0]<-1) {
	        	    break;
	        	}
	        }       	        	        	
		}

		f2[maxf2]--;
    	for(int k= maxf2; k>1; k--) {  //decreasing
       		if (f2[k] < -1) {
          		f2[k-1]--;
     	  		f2[k]=1;
       		}

       		if(f2[0]<-1) {
          		break;
        	}
    	}
	}

	cout << "hello" << endl;
	return 0;
}


/****************************************************************/
/* printAns, printGaps and printVec are simply print formatting functions */
void printAns() {
cout << num << " " << "a:";
    printVec(f1, n);
    cout << endl;
    printVec(f2, n);
    cout << endl;
    printVec(f3, n);
    cout << endl << endl;
    printVec(F, n);
    cout << endl;
    printGaps(F, n);
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








