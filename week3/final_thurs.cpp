////////////////////////////////////////
/* final_thurs.cpp

intermediate code towards the final result seen in 
singlerun_fullalg.cpp. this code lacks the correct
implementation of the prev1, prev2, and prev3 (i.e. the previous
values for the given i, i+35, i+70 in the previous iteration).

(c) Molly Feldman, Summer@ICERM 2014. All Rights Reserved. 
*/
///////////////////////////////////////


#include <iostream>
#include<string>
using namespace std;

int num = 0;
const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int l = 5;
const int r = 1; /*additional value, our n is of the form pqr */

const int maxf2 = 21;
const int maxf3 = 15;
const int maxf1 = maxf2 + maxf3 -1; 

const int maxrepf2 = 3;
const int maxrepf3 = 3;

const int n = p*l*q*r;
int g1[n], g2[n], g3[n], f2[21], f3[15], F[n], inter[n], repf2[7], repf3[5];
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
int baseCases();
void zeroOut(int*);
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
	for (int e=0; e<21; e++) f2[e] = 0;
	for (int f=0; f<15; f++) f3[f] = 0;
	for (int s=0; s<5; s++) repf3[s] = 0;
	for (int t=0; t<7; t++) repf2[t] = 0;
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
	cout << "inter[0] is: " << inter[0] << endl;
	cout << "inter[70] is: " << inter[70] << endl;
	cout << "inter[35] is: " << inter[35] << endl;
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
//////////////////////////////////////////////////////////////////////////////////////////
void zeroOut(int* v) {
	for (int i=0; i<n; i++) {
		g1[i] = 0;
		F[i] = 0;
	}
	baseCases();
}
////////////////////////////////////////////////////////////////////////////////////////////
void reset(int i) {
	g1[i] = 0;
	g1[i+35] = 0;
	g1[i+70] = 0;
	F[i] = 0;
	F[i+35] = 0;
	F[i+70] = 0;

}


/////////////////////////////////////////////////////////////////////////
int produceArray(int i, int prev1, int prev2, int prev3, int result) {
	if (result == 10) {
		return result;
	}
	
	cout << "i: " << i << " " << "result: " << result << " " << "prevs: " << prev1 << " " << prev2 << " " << prev3 << endl;
	if (i==35) {
		printAns();
		//if (!isPeriodic(F,n) && oddNumber(F,n) && checkAlt(F)) printAns();
		//zeroOut(F);
		return result;
	}

	//FORCED CASES
	if (inter[i] == 2) {
		//only one case: can only be -1

		//(-1, 1, -1)
		if ((prev1 != 1) && (inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			set(i,-1,1,-1);
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 result = produceArray(i+1, 1, prev2, prev3,  result);
			 reset(i);	
		}

		reset(i);

		return 1;

		
	}

 	/////////////////////////////////////////
	else if (inter[i] == -2) {
		//only one case: can only be 1

		//(1, -1, 1)
		if ((prev1 != -1) && (inter[i+35] >= 0) && (inter[i+70] <= 0) && (prev2 != inter[i+35] - 1) && (prev3 != inter[i+70]+1)) {
			set(i,1,-1,1);
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			result = produceArray(i+1, -1, prev2, prev3,  result);
			reset(i);
		}
		
		reset(i);
		return 2;
		
	}

	//////////////////////////////////////////
	else if (inter[i] == 1) {
		//two cases: can only be -1 or 0

		//(0, 0, 0)
		if ((prev1 != 1) && (inter[i+35] != 2) && (inter[i+35] != -2) && (inter[i+70] != 2) && (inter[i+70] != -2) && (prev2 != inter[i+35])
			&& (prev3 != inter[i+70])) {
			set(i,0,0,0);
			if (inter[i+35] != 0) prev2 = inter[i+35] ;
			if (inter[i+70] != 0) prev3 = inter[i+70];
			result = produceArray(i+1, 1, prev2, prev3, result);
			reset(i);
		}

		//(-1, 1, -1)
		if ((inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			set(i,-1,1,-1);
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 result =produceArray(i+1, prev1, prev2, prev3,  result);
			 reset(i);	
		}

		reset(i);
		return 3;

		
	}



	/////////////////////////////////////////////////
	else if (inter[i] == -1) {
		//only two cases: can either be 1 or 0

		// (0,0,0)
		if ((prev1 != -1) && (inter[i+35] != 2) && (inter[i+35] != -2) && (inter[i+70] != 2) && (inter[i+70] != -2) && (prev2 != inter[i+35])
			&& (prev3 != inter[i+70])) {
			set(i,0,0,0);
			if (inter[i+35] != 0) prev2 = inter[i+35] ;
			if (inter[i+70] != 0) prev3 = inter[i+70];
			result = produceArray(i+1, -1, prev2, prev3, result);
			reset(i);
		}

		//(1, -1, 1)
		if ((inter[i+35] >= 0) && (inter[i+70] <= 0) && (prev2 != inter[i+35] - 1) && (prev3 != inter[i+70]+1)) {
			set(i,1,-1,1);
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			result = produceArray(i+1, prev1, prev2, prev3,  result);
			reset(i);
		}
		reset(i);
		return 4;
	}

	
	/////////////////////////////////////////////////
	else if (inter[i] == 0) {
		
		//(1, -1, 1)
		if ((prev1 != 1) && (inter[i+35] >= 0) && (inter[i+70] <= 0) && (prev2 != inter[i+35] - 1) && (prev3 != inter[i+70]+1)) {
			cout << "in 0 case: choosing 1" << endl; 
			set(i,1,-1,1);
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			result= produceArray(i+1, 1, prev2, prev3,  result);
			reset(i);
		}

		//(-1, 1, -1)
		if ((prev1 != -1) && (inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			cout << "in 0 case: choosing -1" << endl;
			set(i,-1,1,-1);
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 result = produceArray(i+1, -1, prev2, prev3,  result);
			 reset(i);	
		}

		// (0,0,0)
		if ((inter[i+35] != 2) && (inter[i+35] != -2) && (inter[i+70] != 2) && (inter[i+70] != -2) && (prev2 != inter[i+35])
			&& (prev3 != inter[i+70])) {
			cout << "in 0 case: choosing 0" << endl;
			set(i,0,0,0);
			if (inter[i+35] != 0) prev2 = inter[i+35] ;
			if (inter[i+70] != 0) prev3 = inter[i+70];
			result = produceArray(i+1, prev1, prev2, prev3, result);
			reset(i);
		}
		reset(i);
		return 5;
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
	

	/*f2[0] = f2[8] = f2[13] = f2[14] = f2[17] = f2[19] = 1;
	f2[4] = f2[9] = f2[15] = f2[16] = f2[18] = -1;

	f3[4] = f3[10] = 1;
	f3[0] = f3[7] = f3[13] = -1;*/

	f3[0] = f3[6] = f3[8] = f3[10] = f3[14] = -1;
	f3[4] = f3[7] = f3[9] = f3[11] = 1;

	f2[3] = f2[5] = f2[11] = f2[17] = -1;
	f2[2] = f2[6] = f2[12] = f2[18] = 1;


	repeat(f2, g2, maxf2, n, 1, 0);
	repeat(f3, g3, maxf3, n, 1, 0);
	

	for (int i=0; i<n; i++) {
		inter[i] = g2[i] + g3[i];
	}

	if (!baseCases()) cout << "failed base cases" << endl;
	
	int result = 0;
	result = produceArray(1, 1, -1, 1, result);
	cout << "END RESULT IS: " << result << endl;
	

	/*big:
	while (true) {

		while(true) {
			
	        repeat(repf2, f2, 7, 21, 1, 0);
			repeat(repf3, f3, 5,15, 1, 0);

			repeat(f2, g2, maxf2, n, 1, 0);
			repeat(f3, g3, maxf3, n, 1, 0);

			for (int i=0; i<n; i++) {
				inter[i] = g2[i] + g3[i];
			}
			int result = 0;
		    if (!baseCases()) { 
		    	cout << "failed base cases" << endl;
		    	result = 10;
		    }
	
		
		result = produceArray(1, 1, -1, 1, result);
		cout << "END RESULT IS: " << result << endl << endl;

	  
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
		}*/
	
	/*f2[0] = f2[2] = f2[4] = 1;
	f2[1] = f2[3] = f3[5] = -1;*/

		/*
	
	repeat(f2, g2, maxf2, n, 1, 0);
	repeat(f3, g3, maxf3, n, 1, 0);

	for (int i=0; i<n; i++) {
		inter[i] = g2[i] + g3[i];
	}
	
	l;
	if (!baseCases()) cout << "failed base cases" << endl;
	
	int result = 0;
	result = produceArray(1, 1, -1, 1, result);
	cout << "END RESULT IS: " << result << endl;
	
	//if (oddNumber(F,n) && checkAlt(F))printAns();
	*/

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
        if (i==34 || i== 69) cout << " ";
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








