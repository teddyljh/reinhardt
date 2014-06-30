#include <iostream>
using namespace std;

int num = 0;
const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int l = 5;
const int r = 1; /*additional value, our n is of the form pqr */

const int maxf2 = 15;
const int maxf3 = 21;
const int maxf1 = maxf2 + maxf3 -1; 

const int maxrepf2 = 5;
const int maxrepf3 = 7;

const int n = p*l*q*r;
int g1[n], g2[n], g3[n], g4[n], f2[15], f3[21], f1[35],  Fstart[35], F[n], inter[n], repf2[5], repf3[7];
bool boolc[n][3];
/*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

void printVec(int*, int);
void printGaps(int*, int);
void printAns();
bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);
void setCoeffs();
void repeat(int*, int*, int, int, int, int);
void combine(int* , int* , int*);
void finalStep();
void determine();
bool oddNumber(int*, int);

////////////////////////////////////////////////////////////////////////
/* setCoeffs - basic setup for all coefficient vectors use within */
void setCoeffs() {
	for (int x=0; x < n; x++) {
		for (int y=0; y <n ; y++ ){
			boolc[x][y] = false;
		}
	}
	for (int i=0; i<n; i++) g2[i] = g3[i] = F[i] = inter[n] = 0;
	for (int j=0; j<n; j++) {
		if (j%3 == 0) g1[j] = 0;
		if (j%3 == 1) g1[j] = -1;
		if (j%3 == 2) g1[j] = 1;
	}
	for (int f=0; f< 5; f++) repf2[f] = 1;
	for (int s =0; s<7; s++) repf3[s] = 1;
	for (int e=0; e<15; e++) f2[e] = 0;
	for (int f=0; f<21; f++) f3[f] = 0;
	for (int a=0; a < 35; a++) Fstart[a] = f1[a] = 0;
}
///////////////////////////////////////////////////////////////////////////
/* repeat - repeats the coefficient sequence of v, with alternating sign,
and sets them to be the coefficient sequence of w.
	recursive definition
*/
void repeat(int* v, int* w, int size1, int size2, int num, int rep) {
	//int size1 = sizeof(v)/sizeof(int); 
	//int size2 = sizeof(w)/sizeof(int); //WATCH OUT; PRINT OUT, could give wrong results

	if (rep == size2) {
	//	cout << "at end of repeat: " ;
	//	printVec(w, n);
	//	cout << endl;
		return;
	}
	for (int i = rep; i < size1 + rep; i++ ) {
		w[i] = num*v[i%size1];

	}
	repeat(v, w, size1, size2, -num, rep+size1);

}

//////////////////////////////////////////////////////////////////////////
/* combine - adds three coefficient vectors to determine the final Reinhardt polynomial */
void combine(int* v, int* w, int *x) {
	for (int i=0; i<n; i++) {
		F[i] = v[i] + w[i] + x[i];
	}
}


//////////////////////////////////////////////////////////////////////////
void determine() {

	//we know that F[0] == 1, as that's a necessary property

	if (inter[0] == 0) f1[0] = 1;
	if (inter[0] == -1 || inter[0] == -2) return;

	if (inter[0] == 1) f1[0] = 0;
	if (inter[0] == 2) f1[0] = -1;
	Fstart[0] = 1;

	for (int i=0; i<n; i++) { //defacto rule; there is no other possible value for g1[i] -> F[i]

		if (inter[i] == 2) { 
			f1[i] = -1;
			Fstart[i] = 1;
		}
		if (inter[i] == -2) {
		    f1[i] = 1;
		    Fstart[i] = -1;
		}

	}

}

//////////////////////////////////////////////////////
bool oddNumber(int* F, int n) {
	bool correct = true;
	for (int i=1; i< n; i++) {
		if (F[i] == 1) correct = true;
		if (F[i] == -1) correct = false;


	}
	return correct;

}
//////////////////////////////////////////////////////////////////////////
void finalStep() {

	for (int i=1; i<35; i++) {
		if (Fstart[i] != 0) continue;

		//two ways we can go: if inter[i] == 0 or if inter[i] == -1/+1

		if (inter[i] == 1 || inter[i] == -1) {

			int k=1;

			while (Fstart[i-k] == 0) {
				//cout << k << " ";
				k++;
			}

			if (Fstart[i-k] == -1 && inter[i] == -1) { //forced case
				f1[i] = 1;
				Fstart[i] = 0;
			}

			else if (Fstart[i-k] == 1 && inter[i] == 1) { //forced case
				f1[i] = -1;
				Fstart[i] = 0;

			}

			else if (Fstart[i-k] == 1 && inter[i] == -1) { //can either have F[i] = 0 or F[i] = -1; going to guess -1 is better
				f1[i] = 0;
				Fstart[i] = -1;

			}

			else if (Fstart[i-k] == -1 && inter[i] == 1) { //can either have F[i] = 0 or F[i] = 1; going to guess 1 is better
				f1[i] = 0;
				Fstart[i] = 1;

			}
		}

		
		if (inter[i] == 0){ //in this case, inter[i] = 0 or inter[i] = 2/-2 but that case has already been delt with 
			int w=1;
			while (Fstart[i-w] == 0) {
				
				w++;
			}
			
			//cout << "i: " << i << " w: " << w << endl;
			/*
			if (1 == w) { 
				g1[i] = 0;
				F[i] = 0; 
			}
			else { */
				int r = rand() % 2;
				//cout << "r: " << r << endl;
				if (r == 0 && Fstart[i-w] == -1) {
					f1[i] = 1;
					Fstart[i] = 1;
				}


				else if (r == 1 && Fstart[i-w] == -1) {
					f1[i] = 0;
					Fstart[i] = 0;
				}

				else if (r == 0 && Fstart[i-w] == 1) {
					f1[i] = -1;
					Fstart[i] = -1;

				}

				else if (r == 1 && Fstart[i-w] == 1) {
					f1[i] =0;
					Fstart[i] = 0;
				}
				else {
					f1[i] = 0;
					Fstart[i] = 0;
				}

			//}
	
		}

	
	}
	repeat(f1, g1, maxf1, n, 1, 0);
	repeat(Fstart, F, 35, n, 1, 0);
	//printAns();
	if (!isPeriodic(F,n) && oddNumber(F,n)) printAns();

}





///////////////////////////////////////////////////////////////
int main() {
	
	cout << "START EXECUTION" << endl;
	//sets up the coefficient vectors
	setCoeffs();
	
	big:
	while (true) {

		while(true) {
			
	        repeat(repf2, f2, 5, n, 1, 0);
			repeat(repf3, f3, 7, n, 1, 0);

			repeat(f2, g2, maxf2, n, 1, 0);
			repeat(f3, g3, maxf3, n, 1, 0);

			for (int i=0; i<n; i++) {
				inter[i] = g2[i] + g3[i];
			}
		    //inter is all set up given the definition of g2 and g3 above
		    //now on to the algorithmic determination of g1
		    determine();
		    finalStep();

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

	cout << "all done!" << endl;
	return 0;
}

/****************************************************************/
/* printAns, printGaps and printVec are simply print formatting functions */
void printAns() {
cout << num << endl;
    cout << "g2: ";
    printVec(g2, n);
    cout << endl;
    cout << "g3: ";
    printVec(g3, n);
    cout << endl;
    cout << "+ : ";
    printVec(inter, n);
    cout << endl << endl;
    cout << "g1: ";
    printVec(g1, n);
    cout << endl << endl;
    cout << "F:  ";
    printVec(F, n);
    cout << endl;
    cout << "    ";
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
        else if (v[i] == 2) cout << '2';
        else if (v[i] == -2) cout << '*';
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








