//////////////////////////
/* cycle_all.cpp

This code run through all possible f2 and f3 values,
determine g2/g3 from them, and then determines all possible g1 values
using the algorithm implemented in produceArray. Then it determines
whether any resultant F are valid. If so, it prints out information
about the Reinhardt polymonial/polygon to the user. 

Molly Feldman, Summer@ICERM, 2014. All Rights Reserved. 
(printAns, printGaps, isPeriodic, printVec, hasPeriod 
courtesy of Michael Mossinghoff)

NOTE: If interested in output of cycle_all.cpp, please contact
the creator. 
*/
//////////////////////////



#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

int num = 0; /*total valid number of polynomials */
int num_seen = 0; /* total actually seen */ 
const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int l = 5;
const int r = 1; /*additional value, our n is of the form pqr */

const int maxf2 = 21;
const int maxf3 = 15;

const int n = p*l*q*r;
int g1[n], g2[n], g3[n], f2[21], f3[15], F[n], inter[n];

int prev1 = 0;
int prev2 = 0;
int prev3 = 0;

int result = 0;

int allF[n][n];
/*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

void printVec(int*, int);
void printGaps(int*, int);
void printGapsOrdered(int*, int);
void printAns();
bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);
void setCoeffs();
void repeat(int*, int*, int, int, int, int);
int baseCases();
bool oddNumber(int*, int);
bool checkAlt(int*);
void set(int,int,int,int);
int produceArray(int); 

////////////////////////////////////////////////////////////////////////
/* setCoeffs - basic setup for all coefficient vectors use within */
void setCoeffs() {
	for (int i=0; i<n; i++) g2[i] = g3[i] = inter[i] = 0;
	for (int j=0; j<n; j++) g1[j] = F[j] = 0;
	for (int e=0; e<21; e++) f2[e] = 1;
	for (int f=0; f<15; f++) f3[f] = 1;
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
void set(int i, int iterm, int aterm, int bterm) {
	g1[i] = iterm; F[i] = g1[i] + g2[i] + g3[i];
	g1[i+35]= aterm; F[i+35] = g1[i+35] + g2[i+35] + g3[i+35];
	g1[i+70] = bterm; F[i+70] = g1[i+70] + g2[i+70] + g3[i+70];
}
//////////////////////////////////////////////////////////////////////////
int baseCases() { //sets up holdfast, required values
	//cout << "inter[0] is: " << inter[0] << endl;
	//cout << "inter[70] is: " << inter[70] << endl;
	//cout << "inter[35] is: " << inter[35] << endl;
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
	
	return 1;
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
int produceArray(int i) {
	if (result == 10) {
		
		return result;
	}
	
	//cout << "i: " << i << " " << "result: " << result << " " << "prevs: " << prev1 << " " << prev2 << " " << prev3 << endl;
	if (i==35) {
		//printAns();
		if (!isPeriodic(F,n) && oddNumber(F,n) && checkAlt(F)) printAns();
		num_seen++;
		//zeroOut(F);
		return result;
	}

	//FORCED CASES
	if (inter[i] == 2) {
		//only one case: can only be -1

		//(-1, 1, -1)
		if ((prev1 != 1) && (inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			set(i,-1,1,-1);
			int val1[] = {prev1, prev2, prev3};
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			prev1 = 1;
			result = produceArray(i+1);
			prev1 = val1[0];
			prev2 = val1[1]; 
			prev3 = val1[2];
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
			int val2[] = {prev1, prev2, prev3};
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			prev1 = -1; 
			result = produceArray(i+1);
			prev1 = val2[0]; 
			prev2 = val2[1]; 
			prev3 = val2[2];
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
			int val3[] = {prev1, prev2, prev3};
			if (inter[i+35] != 0) prev2 = inter[i+35] ;
			if (inter[i+70] != 0) prev3 = inter[i+70];
			prev1 = 1;
			result = produceArray(i+1);
			prev1 = val3[0];
			prev2 = val3[1]; 
			prev3 = val3[2];
			reset(i);
		}

		//(-1, 1, -1)
		if ((inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			set(i,-1,1,-1);
			int val4[] = {prev1, prev2, prev3};
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			 result =produceArray(i+1);
			 prev1 = val4[0];
			 prev2 = val4[1];
			 prev3 = val4[2];
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
			int val5[] = {prev1, prev2, prev3};
			if (inter[i+35] != 0) prev2 = inter[i+35] ;
			if (inter[i+70] != 0) prev3 = inter[i+70];
			prev1 = -1; 
			result = produceArray(i+1);
			prev1 = val5[0];
			prev2 = val5[1];
			prev3 = val5[2]; 
			reset(i);
		}

		//(1, -1, 1)
		if ((inter[i+35] >= 0) && (inter[i+70] <= 0) && (prev2 != inter[i+35] - 1) && (prev3 != inter[i+70]+1)) {
			set(i,1,-1,1);
			int val6[] = {prev1, prev2, prev3};
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			result = produceArray(i+1);
			prev1 = val6[0];
			prev2 = val6[1];
			prev3 = val6[2];
			reset(i);
		}
		reset(i);
		return 4;
	}

	
	/////////////////////////////////////////////////
	else if (inter[i] == 0) {
		
		//(1, -1, 1)
		if ((prev1 != 1) && (inter[i+35] >= 0) && (inter[i+70] <= 0) && (prev2 != inter[i+35] - 1) && (prev3 != inter[i+70]+1)) {
			//cout << "in 0 case: choosing 1" << endl; 
			set(i,1,-1,1);
			int val7[] = {prev1, prev2, prev3};
			if (inter[i+35] - 1 != 0) prev2 = inter[i+35] -1;
			if (inter[i+70] + 1 != 0) prev3 = inter[i+70] + 1;
			prev1 = 1; 
			result= produceArray(i+1);
			prev1 = val7[0];
			prev2 = val7[1]; 
			prev3 = val7[2];
			reset(i);
		}

		//(-1, 1, -1)
		if ((prev1 != -1) && (inter[i+35] <= 0) && (inter[i+70] >= 0) && (prev2 != inter[i+35] +1) && (prev3 != inter[i+70] - 1)) {
			//cout << "in 0 case: choosing -1" << endl;
			set(i,-1,1,-1);
			int val8[] = {prev1, prev2, prev3};
			if (inter[i+35] + 1 != 0) prev2 = inter[i+35] +1;
			if (inter[i+70] - 1 != 0) prev3 = inter[i+70] - 1;
			prev1 = -1; 
			 result = produceArray(i+1);
			 prev1 = val8[0];
			 prev2 = val8[1];
			 prev3 = val8[2];
			 reset(i);	
		}

		// (0,0,0)
		if ((inter[i+35] != 2) && (inter[i+35] != -2) && (inter[i+70] != 2) && (inter[i+70] != -2) && (prev2 != inter[i+35])
			&& (prev3 != inter[i+70])) {
			//cout << "in 0 case: choosing 0" << endl;
			set(i,0,0,0);
			int val9[]= {prev1, prev2, prev3};
			if (inter[i+35] != 0) prev2 = inter[i+35] ;
			if (inter[i+70] != 0) prev3 = inter[i+70];
			result = produceArray(i+1);
			prev1 = val9[0];
			prev2 = val9[1]; 
			prev3 = val9[2];
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



////////////////////////////////////////////////////////////////////////////////
int main() {
	
	cout << "START EXECUTION" << endl;
	//sets up the coefficient vectors
	
	
	big:
	while (true) {

		while(true) {
			
			repeat(f2, g2, maxf2, n, 1, 0);
			repeat(f3, g3, maxf3, n, 1, 0);
	
			for (int i=0; i<n; i++) {
				inter[i] = g2[i] + g3[i];
			}

			baseCases();
	
			int result = 0;
			prev1 = 1;
			prev2 = -1; 
			prev3 = 1; 
 
			result = produceArray(1);
			
			
	       	// next iteration:
	       	f3[maxf3]--;
	       	for(int j= maxf3; j>=1; j--) {  //decreasing
	       	   if (f3[j] < -1) {
	           	f3[j-1]--;
	     	   	f3[j]=1;
	        	}

	        	if(f3[0]<-1) {
	        	    f3[0] = 1;
	        	    goto outside;

	        	}
	        }       	        	        	
		}
		outside:
			f2[maxf2]--;
    		for(int k= maxf2; k>=1; k--) {  //decreasing
       			if (f2[k] < -1) {
          			f2[k-1]--;
     	  			f2[k]=1;
       			}

       			if(f2[0]<-1) {
       				f2[0] =1;
          			goto big;
        		}
    		}
		}

	cout << "TOTAL NUMBER OF POLYNOMIALS SEEN: " << num_seen << endl;
	cout << "TOTAL NUMBER OF VALID POLYNOMIALS: " << num << endl;

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
    cout << endl;
    cout << endl;
     cout << "    ";
    printGapsOrdered(F,n);
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

void printGapsOrdered(int* v, int n) { /*gives us the actual "list sequence" of numbers for the Reinhardt angle differences*/
    int allF[n];
    for (int i=0; i<n; i++) allF[i] = 0;
    int k = 0, gap;
    while (++k < n) {
        gap = 0;
        for (gap=0; k < n && v[k] == 0; k++) gap++;
        allF[k-1] = gap+1;
        //cout << " " << (gap+1);
    }
    int large = 0;
    int start = -1; 
    for (int j=0; j<n; j++) {
    	if (allF[j] > large) {
    		large = allF[j];
    		start = j;
    	}
    }

    for (int h=start; h< n; h++) {
    	if (allF[h] == 0) continue;
    	cout << " " << allF[h];
    }
    for (int g=0; g<start; g++) {
    	if (allF[g] == 0) continue;
    	cout << " " << allF[g];
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