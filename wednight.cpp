#include <iostream>
#include<string>
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
int explicitCases(int);
int baseCases();
void zeroOut(int*);
int determine(int);
bool oddNumber(int*, int);
bool checkAlt(int*);
void set(int,int,int,int);
int produceArray(int, int , int , int ,int); 

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
	if (rep == size2) {
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
void set(int i, int iterm, int aterm, int bterm) {
	g1[i] = iterm; F[i] = g1[i] + g2[i] + g3[i];
	g1[i+35]= aterm; F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
	g1[i+70] = bterm; F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
}
//////////////////////////////////////////////////////////////////////////
int baseCases() { //sets up holdfast, required values

	//we know that F[0] == 1, as that's a necessary property
	if (inter[0] == -1 || inter[0] == -2) return 0;


	if (inter[0] == 0)  { 
		if (inter[35] == -2 || inter[70] == 2 || inter[70] == 1 || inter[35] == -1) return 0; 
		set(0, 1, -1, 1);
	}
	

	else if (inter[0] == 1) { 
		if (inter[35] == -2 || inter[35] == 2 || inter[70] == -2 || inter[70] == 2) return 0; //0 gives us a bad F
		set(0, 0, 0, 0);
	}
	else if (inter[0] == 2) { 

		if (inter[35] == 2 || inter[70] == -2 || inter[35] == 1 || inter[70] == -1) return 0;
		set(0, -1, 1, -1);
	}
	done[0] = done[35] = done[70] = true;
	return 1;
}

void zeroOut(int* v) {
	for (int i=0; i<n; i++) {
		g1[i] = 0;
		F[i] = 0;
	}
	baseCases();
}
/////////////////////////////////////////////////////////////////////////
int produceArray(int i, int prev1, int prev2, int prev3, int result) {
	cout << "i: " << i << " " << "result: " << result << " " << "prevs: " << prev1 << " " << prev2 << " " << prev3 << endl;
	if (i==35) {
		//printAns();
		if (!isPeriodic(F,n) && oddNumber(F,n) && checkAlt(F)) printAns();
		//zeroOut(F);
		return result;
	}
	//FORCED CASES
	if (inter[i] == 2) {
		cout << "in inter[i] == 2" << endl;
		if (prev1 == 1) result = 1; // going to ruin alternating
		if ((inter[i+35] >= 1) || (inter[i+70] <= -1)) result = 2; //going to ruin coefficients in -1,0, 1
		if ((prev2 == inter[i+35] +1) || (prev3 == inter[i+70]-1)) result = 3; //going to ruin alternating
		set(i, -1, 1, -1);
		if (inter[i] -1 != 0) prev1 = inter[i] - 1;
		if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
		if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
		produceArray(i+1, prev1, prev2, prev3,  result);
		
	}
 	/////////////////////////////////////////
	else if (inter[i] == -2) {
		if (prev1 == -1) result =4; //i is alternating
		if ((inter[i+35] <= -1) || (inter[i+70] >= 1)) result  = 5; //coefficients
		if ((prev2 == inter[i+35]-1) || (prev3 == inter[i+70]+1)) result =6; //i is alternating
		set(i, 1, -1, 1);
		if (inter[i] +1 != 0) prev1 = inter[i] +1;
		if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
		if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
		produceArray(i+1, prev1, prev2, prev3,  result);
	}
	//////////////////////////////////////////
	else if (inter[i] == 1) {
		//two cases: g1 can either be -1 or g1 can be 0
		if (prev1  == 1) { //forced case; g1[i] must be -1
			if ((prev2 == inter[i+35]+1) || (prev3 == inter[i+70]-1)) result  = 7; //i is alternating
			if ((inter[i+35] >= 1) || (inter[i+70] <= -1)) result = 8; //coefficients
			set(i, -1, 1, -1);
			if (inter[i] -1 != 0) prev1 = inter[i] - 1;
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 produceArray(i+1, prev1, prev2, prev3,  result);
		}
		else if (prev1 == -1) {
			if ((prev2 == inter[i+35]+1) || (prev3 == inter[i+70]-1)) { //then -1 case isn't going to work, so don't do it
				set(i,0,0,0);
				 produceArray(i+1, prev1, prev2, prev3, result);
			}
			else if ((inter[i+35] >= 1) || (inter[i+70] <= -1)) { //coefficients are going to be wrong
				set(i,0,0,0);
				 produceArray(i+1, prev1, prev2, prev3, result);
			}
			set(i,-1,1,-1);
			if (inter[i] -1 != 0) prev1 = inter[i] - 1;
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 produceArray(i+1, prev1, prev2, prev3,  result);

			set(i,0,0,0);
			 produceArray(i+1, prev1, prev2, prev3,  result);
			//DON'T KNOW HOW TO MAKE THIS WORK IF BOTH ARE POSSIBLE

		}

	}
	/////////////////////////////////////////////////
	else if (inter[i] == -1) {
		//two cases: g1 can either be -1 or g1 can be 0
		if (prev1  == -1) { //forced case; g1[i] must be 1
			if ((prev2 == inter[i+35]-1) || (prev3 == inter[i+70]+1)) result = 9; //i is alternating
			if ((inter[i+35] <= -1) || (inter[i+70] >= 1)) result = 10; //coefficients
			set(i, 1, -1, 1);
			if (inter[i] +1 != 0) prev1 = inter[i] + 1;
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] - 1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			produceArray(i+1, prev1, prev2, prev3,  result);
		
		}
		else if (prev1 == 1) {
			if ((prev2 == inter[i+35]-1) || (prev3 == inter[i+70]+1)) { //then -1 case isn't going to work, so don't do it
				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3, result);
			}
			else if ((inter[i+35] <= -1) || (inter[i+70] >= 1)) { //coefficients are going to be wrong
				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3, result);
			}
			else {
				set(i,1,-1,1);
				if (inter[i] +1 != 0) prev1 = inter[i] + 1;
				if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
				if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
				produceArray(i+1, prev1, prev2, prev3,  result);

				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3,  result);
			 	//DON'T KNOW HOW TO MAKE THIS WORK IF BOTH ARE POSSIBLE
			}
		}

	}
	/////////////////////////////////////////////////
	else if (inter[i] == 0) {
		
		//(1, -1, 1)
		if ((inter[i+35] >= 0) && (inter[i+70] <= 0) && (prev2 != inter[i+35] - 1) && (prev3 != inter[i+70]+1)) {
			set(i,1,-1,1);
			if (inter[i] +1 != 0) prev1 = inter[i] + 1;
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			produceArray(i+1, prev1, prev2, prev3,  result);
		}

		//(-1, 1, -1)
		if ((inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			set(i,-1,1,-1);
			if (inter[i] -1 != 0) prev1 = inter[i] - 1;
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 produceArray(i+1, prev1, prev2, prev3,  result);	
		}

		// (0,0,0)
		if ((inter[i+35] != 2) && (inter[i+35] != 2) && (inter[i+70] != 2) && (inter[i+70] != -2) && (prev2 != inter[i+35])
			&& (prev3 != inter[i+70])) {
			set(i,0,0,0);
			produceArray(i+1, prev1, prev2, prev3, result);
		}
}
		/*
		if ((inter[i+35] <=  -1) || (inter[i+70] >= 1)) { //can't do the (1, -1, 1) case
			if ((prev2 == inter[i+35] +1) || (prev3 == inter[i+70] -1)) { //can't do the (-1, 1, -1) case
				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3,  result);
			}
			else {
				set(i,-1,1,-1);
				if (inter[i] -1 != 0) prev1 = inter[i] - 1;
				if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
				if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
				produceArray(i+1, prev1, prev2, prev3,  result);
			}
		}
		else if ((inter[i+35] >= 1) || (inter[i+70] <= -1)) { //can't do the (-1, 1, -1) case
			if ((prev2 == inter[i+35] - 1) || (prev3 == inter[i+70] +1)) { //can't do the (1, -1, 1) case
				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3,  result);
			}
			else {
				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3, result);

				set(i,1,-1,1);
				if (inter[i] +1 != 0) prev1 = inter[i] + 1;
				if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
				if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
				produceArray(i+1, prev1, prev2, prev3,  result);
			}
		}
		else if ((prev2 == inter[i+35] +1) || (prev3 == inter[i+70] -1)) { //can't do the (-1, 1, -1) case
			if ((inter[i+35] >= 1) || (inter[i+70] <= -1)) { //can't do the (1, -1, 1) case
				set(i,0,0,0);
				produceArray(i+1, prev1, prev2, prev3,  result);
			}

		}


		set(i,0,0,0);
		produceArray(i+1, prev1, prev2, prev3, result);

		set(i,1,-1,1);
		if (inter[i] +1 != 0) prev1 = inter[i] + 1;
		if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
		if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
		produceArray(i+1, prev1, prev2, prev3,  result);

		set(i,-1,1,-1);
		if (inter[i] -1 != 0) prev1 = inter[i] - 1;
		if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
		if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
		produceArray(i+1, prev1, prev2, prev3,  result);
	}
	*/

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
	int sign = 1;
	
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
	
	
	for (int i=0; i<maxf2; i++) {
		if (i%2 == 0) f2[i] = 1;
		else f2[i] = -1;
	
	}
	f3[5] = 1; f3[6] = -1; f3[1] = 1; f3[2] = -1;

 	
	repeat(f2, g2, maxf2, n, 1, 0);
	repeat(f3, g3, maxf3, n, 1, 0);

	for (int i=0; i<n; i++) {
		inter[i] = g2[i] + g3[i];
	}
	
	cout << " N IS: " << n << endl;
	if (!baseCases()) cout << "failed base cases" << endl;
	int result = 0;
	result = produceArray(1, 1, -1, 1, result);
	cout << "END RESULT IS: " << result << endl;
	
	//if (oddNumber(F,n) && checkAlt(F))printAns();
	

	cout << "all done" << endl;
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

void printVec(int* v, int n) { /*gives us our result sequence of plus and minuses to print out*/
    for (int i=0; i<n; i++) {
        if (v[i] == 1) cout << '+';
        else if (v[i] == 2) cout << '2';
        else if (v[i] == -2) cout << '*';
        else if (v[i] == -1) cout << '-';
        else cout << v[i];
        if (i==34 || i==69) cout << " ";
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








