#include <iostream>
using namespace std;

int num = 0;
const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int l = 5;
const int r = 1; /*additional value, our n is of the form pqr */

const int maxf2 = 5;
const int maxf3 = 10;
const int maxf1 = maxf2 + maxf3 -1; 

const int maxrepf2 = 5;
const int maxrepf3 = 7;

const int n = p*l*q*r;
int g1[n], g2[n], g3[n], g4[n], f2[15], f3[21], f1[35],  F[n], repf2[5], repf3[7];
bool boolc[n][3];
/*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

void printVec(int*, int);
void printGaps(int*, int);
void printAns();
bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);
void setCoeffs();
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
	for (int e=0; e<15; e++) f2[e] = 1;
	for (int f=0; f<21; f++) f3[f] = 1;
	for (int m=0; m<5; m++) repf2[m] =1;
	for (int n1=0; n1<7; n1++) repf3[n1] = 1;
	//for (int h=0; h<35; h++) f1[h] = 0;
}
///////////////////////////////////////////////////////////////////////////
/* repeat - repeats the coefficient sequence of v, with alternating sign,
and sets them to be the coefficient sequence of w.
	recursive definition
*/
void repeat(int num, int rep) {
	int size1 = sizeof(f2)/sizeof(int); 
	int size2 = sizeof(f3)/sizeof(int); //WATCH OUT; PRINT OUT, could give wrong results

	if (rep == size2) {
		return;
	}

	for (int i = rep; i < (size2/size1 + rep); i++ ) {
		w[i] = num*v[i];

	}
	repeat(-num, rep+size1);

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
		g1[start] = 1;
		finish(start+1);
	}

}

///////////////////////////////////////////////////////////////
int main() {
	
	cout << "START EXECUTION" << endl;
	//sets up the coefficient vectors
	setCoeffs();
	
	f2[0] = f2[8] = f2[13] = f2[14] = f2[17] = f2[19] = 1;
	f2[4] = f2[9] = f2[15] = f2[16] = f2[18] = -1;
	f3[0] = f3[7] = f3[13] = -1;
	f3[4] = f3[10] = 1;
	
	repeat(1, 0);
	printVec(g2, n);
	cout << endl;
	repeat(1, 0);
	printVec(g3, n);
	cout << endl;
	determineBool(g2, g3);
	finish(0);
	/*big:
	while (true) {

		while(true) {
			//printVec(repf2, 5);
			//cout << endl;
			//printVec(repf3, 7);
			//cout << endl;
			//cout << endl;
	        repeat(repf2, f2, 1, 0);
			repeat(repf3, f3, 1, 0);
			repeat(f2, g2, 1, 0);
			repeat(f3, g3, 1, 0);

			determineBool(g2, g3);
			finish(0);

	       	// next iteration:
	       	repf3[maxrepf3]--;
	       	for(int j= maxrepf3; j>=1; j--) {  //decreasing
	       	   if (repf3[j] < -1) {
	           	repf3[j-1]--;
	     	   	repf3[j]=1;
	        	}

	        	if(repf3[0]<-1) {
	        	    repf3[0] = 1;
	        	    goto outside;

	        	}
	        }       	        	        	
		}
		outside:
			repf2[maxrepf2]--;
    		for(int k= maxrepf2; k>=1; k--) {  //decreasing
       			if (repf2[k] < -1) {
          			repf2[k-1]--;
     	  			repf2[k]=1;
       			}

       			if(repf2[0]<-1) {
       				repf2[0] =1;
          			goto big;
        		}
    		}
		}
	*/
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








