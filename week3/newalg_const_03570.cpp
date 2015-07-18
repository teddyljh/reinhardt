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
int g1[n], g2[n], g3[n], g4[n], f2[15], f3[21], F[n], inter[n], repf2[5], repf3[7];
bool done[n];
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
int explicitCases();
int baseCases();
int recursiveCases(int);
bool oddNumber(int*, int);
bool checkAlt(int*);
void set(int,int,int,int);

////////////////////////////////////////////////////////////////////////
/* setCoeffs - basic setup for all coefficient vectors use within */
void setCoeffs() {
	for (int x=0; x < n; x++) {
		for (int y=0; y <n ; y++ ){
			boolc[x][y] = false;
		}
	}
	for (int i=0; i<n; i++) g2[i] = g3[i] = inter[i] = 0;
	for (int j=0; j<n; j++) g1[j] = F[j] = 0;
	for (int h=0; h<n; h++) done[h] = false;
	for (int f=0; f< 5; f++) repf2[f] = 1;
	for (int s =0; s<7; s++) repf3[s] = 1;
	for (int e=0; e<15; e++) f2[e] = 0;
	for (int f=0; f<21; f++) f3[f] = 0;
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
int baseCases() { //sets up holdfast, required values

	//we know that F[0] == 1, as that's a necessary property
	if (inter[0] == -1 || inter[0] == -2) return 0;


	if (inter[0] == 0)  { 
		g1[0] = 1;
		if (inter[35] == -2 || inter[70] == 2) return 0;
		g1[35] = -1; 
		g1[70] = 1;
		F[35] = g1[35] + g2[35] + g3[35];
		F[70] = g1[70] + g2[70] + g3[70];
	}
	

	else if (inter[0] == 1) { 
		g1[0] = 0;
		if (inter[35] == -2 || inter[35] == 2 || inter[70] == -2 || inter[70] == 2) return 0;
		g1[35] = g1[70] = 0;
		F[35] = g1[35] + g2[35] + g3[35];
		F[70] = g1[70] + g2[70] + g3[70];
	}
	else if (inter[0] == 2) { 
		g1[0] = -1;
		if (inter[35] == 2 || inter[70] == -2) return 0;
		g1[35] = 1;
		g1[70] = -1;
		F[35] = g1[35] + g2[35] + g3[35];
		F[70] = g1[70] + g2[70] + g3[70];
	}

	F[0] = 1;
	done[0] = done[35] = done[70] = true;

	
	for (int i=0; i<n; i++) { //defacto rule; there is no other possible value for g1[i] -> F[i]

		if (inter[i] == 2) { 
			g1[i] = -1;
			if (g1[i+35] == 2 || g1[i+70] == -2) return 0;
			g1[i+35] = 1;
			g1[i+70] = -1;
			F[i] = 1;
			F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
		    F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
		    done[i] = done[i+35] = done[i+70] = true;
		}
		if (inter[i] == -2) {
		    g1[i] = 1;
		    if (g1[i+35] == -2 || g1[i+70] == 2) return 0;
		    g1[i+35] = -1; 
		    g1[i+70] = 1;
		    F[i] = -1;
		    F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
		    F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
		    done[i] = done[i+35] = done[i+70] = true;
		}

	}
	
	cout << endl << endl;
	cout << "AFTER BASE: ";
	printVec(g1,n);
	return 1;
}


//////////////////////////////////////////////////////////////////////////
int explicitCases() {

	for (int i=1; i<35; i++) {
		if (F[i] != 0) continue;

		//two ways we can go: if inter[i] == 0 or if inter[i] == -1/+1
		int k1 = 1;
		int k2 = 1;
		int k3 =1;


		while (F[i-k1] == 0) k1++; 
		
		while (F[i+35 -k2] == 0) k2++;

		while(F[i+ 70 - k3] == 0) k3++;
				
		int a= i+35;
		int b= i+70;
		int c = i+35 - k2;
		int d = i+70 - k3;


		if (inter[i] == 1 || inter[i] == -1) {

			if (F[i-k1] == -1 && inter[i] == -1) { //forced case
				g1[i] = 1; F[i] = 0;
				//checks to make sure F coefficients are only 1, -1, 0
				if (inter[i+35] <= -1 || inter[i+70] >= 1) { cout << "NOT GONNA WORK" << endl; return 0; }
				//checks to make sure the terms are alternating
				if (F[i+35-k2] == -1 || F[i+70-k3] == 1) { cout << "NOT GONNA WORK" << endl; return 0;
				g1[i+35] = -1; g1[i+70] = 1;
				F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
				F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
				done[i] = done[i+35] = done[i+70] = true;
			}

			else if (F[i-k1] == 1 && inter[i] == 1) { //forced case
				g1[i] = -1; F[i] = 0;
				//checks to make sure the F coefficients are only 1, -1, 0
				if (inter[i+35] >= 1 || inter[i+70] <= -1) {cout << "NOT GONNA WORK" << endl; return 0; }
				//checks to make sure the terms are alternating
				if (F[i+35-k2] == 1 || F[i+70 - k3] == -1) { cout << "NOT GOOD WORK" << endl; return 0; } 
				g1[i+35] = 1; 
				g1[i+70] = -1;
				F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
				F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
				done[i] = done[i+35] = done[i+70] = true;
			}
		}
			//////////////////////////////////////////////////////////////////////
			//no forced cases: when -1 and 1
			//ONLY THE EXPLICIT CASES (DOES NOT DEAL WITH THOSE BASED ON PROBABILITIES)
		else if (inter[i] == -1 && F[i-k1] == 1) {
			if (F[c] == 1) {
				if (F[d] == 1) {
					if (inter[a] == inter[b] == -1) { //only the zero case
							g1[i] = 0; F[i] = -1;
							g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
							g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
							done[i] = done[i+35] = done[i+70] = true;
					}
					else if (inter[a] == 0 && inter[b] == -2) { //-2, so only -1/1 case
							g1[i] = 1; F[i] = 0;
							g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
							g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
							done[i] = done[i+35] = done[i+70] = true;
					}
				}
				else if (F[d] == -1) {
					if (inter[a] == -1 && inter[b] == 1) { //only the 0 case
							g1[i] = 0; F[i] = -1;
							g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
							g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
							done[i] = done[i+35] = done[i+70] = true;
					}
				}
				else if (inter[a] == -1 && inter[b] == 0) { //only relies on F[c]; one case
						g1[i] = 0; F[i] = -1;
						g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
				}

			}
			if (F[c] == -1) {
				if (F[d] == 1) {
					if (inter[a] == 2 && inter[b] == -2) {
						g1[i] = 1; F[i] = 0;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
					}
				} 
				else if (F[d] == -1) {
					if (inter[a] == 2 && inter[b] == 0) {
						g1[i] = 1; F[i] = 0;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
 					}
 					else if (inter[a] == inter[b] == 1) {
 						g1[i] = 0; F[i] = -1;
						g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
 					}

				}
				if (inter[a] == 2 && inter[b] == -1) { //only relies on F[c]
					g1[i] = 1; F[i] = 0;
					g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
				}


			}
			if (F[d] == -1) {
				if (inter[a] == 0 && inter[b] == 1) {
					g1[i] = 0; F[i] = -1;
					g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
				}
			}
			if (F[d] == 1) {
				if (inter[a] == 1 && inter[b] == -2) {
					g1[i] = 1; F[i] = 0;
					g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
				}
			}

		}

		////////////////////////////////////////////////////////
		else if (inter[i] == 1 && F[i-k1] == -1) {
			if (F[c] == 1) {
				if (F[d] == 1) {
					if (inter[a] == inter[b] == -1) { //only the zero case
							g1[i] = 0; F[i] = 1;
							g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
							g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
							done[i] = done[i+35] = done[i+70] = true;
					}
					
					else if (inter[a] == -2 && inter[b] == 0) { //-2, so only -1/1 case
							g1[i] = -1; F[i] = 0;
							g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
							g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
							done[i] = done[i+35] = done[i+70] = true;
					}
				}
				
				else if (F[d] == -1) {
					if (inter[a] == -2 && inter[b] == 2) { //only the 0 case
							g1[i] = -1; F[i] = 0;
							g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
							g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
							done[i] = done[i+35] = done[i+70] = true;
					}
				}
				
				else if (inter[a] == -2 && inter[b] == 1) { //only relies on F[c]; one case
						g1[i] = -1; F[i] = 0;
						g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
				}


			}
			
			if (F[c] == -1) {
				if (F[d] == 1) {
					if (inter[a] == 1 && inter[b] == -1) {
						g1[i] = 0; F[i] = 1;
						g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
					}
				} 
				
				else if (F[d] == -1) {
					if (inter[a] == 0 && inter[b] == 2) {
						g1[i] = -1; F[i] = 0;
						g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
 					}
 					else if (inter[a] == inter[b] == 1) {
 						g1[i] = 0; F[i] = 1;
						g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
 					}
				}
				
				if (inter[a] == 1 && inter[b] == 0) { //only relies on F[c]
					g1[i] = 0; F[i] = 1;
					g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
				}


			}
			
			if (F[d] == -1) {
				if (inter[a] == -1 && inter[b] == 2) {
					g1[i] = -1; F[i] = 0;
					g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
				}
			}
			if (F[d] == 1) {
				if (inter[a] == 0 && inter[b] == -1) {
					g1[i] = 0; F[i] = 1;
					g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
				}
			}






		}

		//ZERO CASES (NEED TO FIX THOSE/DEAL WITH THOSE LATER)
		else if (inter[i] == 0){ //in this case, inter[i] = 0 or inter[i] = 2/-2 but that case has already been delt with 
			if (F[c] == 1) {
				if (F[d] == 1) {
					if (inter[a] == 0 && inter[b] == -2) {
						g1[i] = 1; F[i] = 1;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
					if (inter[a] == -1 == inter[b]) {
						g1[i] = 0; F[i] = 0;
						g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
					if (inter[a] == -2 && inter[b] == 0) {
						g1[i] = -1; F[i] = -1;
						g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
				}
				if (F[d] == -1) {
					if (inter[a] == 2 && inter[b] == 0) {
						g1[i] = 1; F[i] = 1;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
				}
				if (inter[a] == -2 && inter[b] == 1) {
					g1[i] = -1; F[i] = -1;
					g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
					g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
					done[i] = done[i+35] = done[i+70] = true;
					cout << "ran!" << endl;
				}
			}
			if (F[c] == -1) {
				if (F[d] == 1) {
					if (inter[a] == 2 && inter[b] == -2) {
						g1[i] = 1; F[i] = 1;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
				}
				if (F[d] == -1) {
					if (inter[a] == 0 && inter[b] == 2) {
						g1[i] = -1; F[i] = -1;
						g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
					if (inter[a] == 1 == inter[b]) {
						g1[i] = 0; F[i] = 0;
						g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
					}
				}
				if (inter[a] == 2 && inter[b] == -1) {
					    g1[i] = 1; F[i] = 1;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
				}
			}
			if (F[d] == -1) {
				if (inter[a] == -1 && inter[b] == 2) {
						g1[i] = -1; F[i] = -1;
						g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
				}
			}
			if (F[d] == 1) {
				if (inter[a] == 1 && inter[b] == -2) {
						set(i, 1, -1, 1);
						g1[i] = 1; F[i] = 1;
						g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
						g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
						done[i] = done[i+35] = done[i+70] = true;
						cout << "ran!" << endl;
				}
			}

		}
		else if (inter[a] == -2 && F[i-k1] == -1) return 0;
		else if (inter[a] == 2 && F[i-k1] == 1) return 0;
		else if (inter[a] == -2 && F[i-k1] == 1) {
			set(i, 1, -1, 1);
		}
		else if (inter[a] == 2 && F[i-k1] == -1) {
			set(i, -1, 1, -1);
		}

	
	}
	cout << endl << endl;
	cout << "AFTER EXPLICIT: ";
	printVec(g1,n);
	return 1;
	//printAns();
	//if (!isPeriodic(F,n) && oddNumber(F,n)) printAns();

}
}

void set(int i, int iterm, int aterm, int bterm) {
	g1[i] = iterm; F[i] = g1[i] + g2[i] + g3[i];
	g2[i+35]= aterm; F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
	g2[i+70] = bterm; F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
}
///////////////////////////////////////////////////////////////////
int recursiveCases(int i){
//printAns();
//cout << i << endl;
//cout << n << endl;
//cout << i << endl << endl;
if (done[i]) {cout << "skip" << " " << i << " " << endl; recursiveCases(i+1);}
cout << (i == n) << " " << i << " " << n << endl;
//this function deals with all cases where the results are not totally explicit 
if (i == n) {
	cout << "in final if" << endl;
	printAns();
	return 1;
	//if (!isPeriodic(F,n) && oddNumber(F,n)) printAns();
}
//cout << i << " " << inter[i] << " " << g1[i] << endl;
//cout << inter[i] << endl;
int k1 = 1;
int k2 = 1;
int k3 =1;

while (F[i-k1] == 0) k1++; 		
while (F[i+35 -k2] == 0) k2++;
while (F[i+ 70 - k3] == 0) k3++;
					
int c = i+35 - k2;
int d = i+70 - k3; 
int a = i+35;
int b = i+70;
//cout << a << " " << b << " " << c << " " << endl;

if (inter[i] == -1 && F[i-k1] == 1) { //NEGATIVE ONE CASE
	if (F[c] == 1) {
		if (inter[a] == 0 && inter[b] == -1) {
			g1[i] = 1; F[i] = 0;
			g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
		if (F[d] == -1) {
			if (inter[a] == 0 == inter[b]) {
				g1[i] = 1; F[i] = 0;
				g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
		}
	}
	if (F[c] == -1) {
		if (inter[a] == 1 && inter[b] == 0) {
			g1[i] = 0; F[i] = -1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
		if (F[d] == 1) {
			if (inter[a] == 1 && inter[b] == -1) {
				g1[i] = 0; F[i] = -1;
				g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			} 
		}
	}
	if (F[d] == 1) {
		if (inter[a] == 0 && inter[b] == -1) {
			g1[i] = 0; F[i] = -1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (F[d] == -1) {
		if (inter[a] == 1 && inter[b] == 0) {
			g1[i] = 1; F[i] = 0;
			g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (1) {
		if (inter[a] == 0 && inter[b] == 0) {
			g1[i] = 0; F[i] = -1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (1) {
		if (inter[a] == 1 && inter[b] == -1) {
			g1[i] = 0; F[i] = -1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}

}

else if (inter[i] == 1 && F[i-k1] == -1) {
	if (F[c] == 1) {
		if (F[d] == -1) {
			if (inter[a] == -1 && inter[b] == 1) {
				g1[i] = 0; F[i] = 1;
				g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
		}
		if (inter[a] == -1 && inter[b] == 0) {
			g1[i] = 0; F[i] = 1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
		
	}
	if (F[c] == -1) {
		if (F[d] == 1) {
			if (inter[a] == 0 == inter[b]) {
				g1[i] = -1; F[i] = 0;
				g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
		}
		if (inter[a] == 0 && inter[b] == 1) {
			g1[i] = -1; F[i] = 0;
			g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (F[d] == 1){
		if (inter[a] == -1 && inter[b] == 0) {
			g1[i] = -1; F[i] = 0;
			g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (F[d] == -1) {
		if (inter[a] == 0 && inter[b] == 1) {
			g1[i] = 0; F[i] = 1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (true) {
		if (inter[a] == 0 == inter[b]) {
			g1[i] = 0; F[i] = 1;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (true) {
		if (inter[a] == -1 && inter[b] == 1){
			g1[i] = -1; F[i] = 0;
			g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}

}
else if (inter[i] == 0) { //recursive cases for inter[i] == 0
	if (F[c] == 1) {
		if (F[d] == -1) {
			if (inter[a] == 0 == inter[b]) {
				g1[i] = 1; F[i] = 1;
				g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
			if (inter[a] == -1 && inter[b] == 1) {
				g1[i] = 0; F[i] = 0;
				g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
		}
		if (inter[a] == -1 && inter[b] == 0) {
			g1[i] = 0; F[i] = 0;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
		if (inter[a] == 0 && inter[b] == -1) {
			g1[i] = 1; F[i] = 1;
			g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
	}
	if (F[c] == -1) {
		if (F[d] == 1) {
			if (inter[a] == 0 == inter[b]) {
				g1[i] = -1; F[i] = -1;
				g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
			if (inter[a] == 1 && inter[b] == -1) {
				g1[i] = 0; F[i] = 0;
				g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
			}
		}
		if (inter[a] == 0 && inter[b] == 1) {
			g1[i] = 1; F[i] = 1;
			g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}
		if (inter[a] == 1 && inter[b] == 0) {
			g1[i] = 0; F[i] = 0;
			g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
			g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
			recursiveCases(i+1);
		}

	}
	if (F[d] == 1) {
		if (inter[a] == 0 && inter[b] == -1) {
			g1[i] = 0; F[i] = 0;
				g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
		}
		if (inter[a] == -1 && inter[b] == 0) {
				g1[i] = -1; F[i] = -1;
				g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
		}
	}
	if (F[d] == -1) {
		if (inter[a] == 0 && inter[b] == 1) {
				g1[i] = 0; F[i] = 0;
				g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
		}
		if (inter[a] == 1 && inter[b] == 0) {
				g1[i] = 1; F[i] = 1;
				g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
				g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
				recursiveCases(i+1);
		}
	}
	if (inter[a] == 1 && inter[b] == -1) {
		g1[i] = 1; F[i] = 1;
		g1[a] = -1; F[a] = g1[a] + g2[a] + g3[a];
		g1[b] = 1; F[b] = g1[b] + g2[b] + g3[b];
		recursiveCases(i+1);
	}
	if (inter[a] == -1 && inter[b] == 1) {
		g1[i] = -1; F[i] = -1;
		g1[a] = 1; F[a] = g1[a] + g2[a] + g3[a];
		g1[b] = -1; F[b] = g1[b] + g2[b] + g3[b];
		recursiveCases(i+1);
	}
	if (inter[a] == 0 == inter[b]) {
		g1[i] = 0; F[i] = 0;
		g1[a] = 0; F[a] = g1[a] + g2[a] + g3[a];
		g1[b] = 0; F[b] = g1[b] + g2[b] + g3[b];
		recursiveCases(i+1);
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
////////////////////////////////////////////////////////////////
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
	return true;
	}
}


///////////////////////////////////////////////////////////////
int main() {
	
	cout << "START EXECUTION" << endl;
	//sets up the coefficient vectors
	setCoeffs();
	

	// values set for a known polynomial that gives us a n=105 polygon
	for (int i=0; i<10; i++) {

	f2[(i%5)*3] = f2[6] = f2[10] = 1;
	f2[1] = f2[5] = f2[11] = -1;
	
	f3[0] = f3[3] = f3[(i%2) * 9] = f3[(i%5) * 13] = f3[14] = f3[18] = 
	1;
	f3[(i%6) * 2] = f3[6] = f3[7] = f3[12] = f3[(i%3) * 7] = f3[20] = 
	-1;

 	
	repeat(f2, g2, maxf2, n, 1, 0);
	repeat(f3, g3, maxf3, n, 1, 0);
	//printVec(g2, n); 
	//cout << endl;
	//printVec(g3, n);

	for (int i=0; i<n; i++) {
		inter[i] = g2[i] + g3[i];
	}
	printVec(inter, n);
	cout << endl;

	
	if (!baseCases()) cout << "failed base cases" << endl;
	if (!explicitCases()) cout << "failed explicit cases" << endl;
	recursiveCases(1);
	

	cout << "all done" << endl;
	cout << "////////////////////////////////////////////////" << endl << endl;
}
	//inter is all set up given the definition of g2 and g3 above
	//now on to the algorithmic determination of g1
	//if (baseCases()) 
	//	if (explicitCases()) cout << "all done!" << endl;
	//else {
	//	cout << "failed" << endl;
	//}

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
        if (i==35 || i==70) cout << " ";
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








