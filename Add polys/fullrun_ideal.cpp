/*
fullrun_ideal.cpp
this program determines possible sporadic Reinhardt n-gons
by cycling through all possible f2 and f3 values, constructing the
associated g2/g3 sequences and then, according to the necessary 
restrictions, constructs all possible g1 and thus F. If an F 
represents a valid Reinhardt polynomial, the result is then outputed
 to the user. In the "ideal" version,F is printed out to the user in 
 a normalized form, for ease of comparing.
 
 
 [MAY HAVE FIXED THIS PROBLEM BUT THE DISCLAIMER STANDS]
 This program only finds valid polygons where g1 has a 1 in
 it's first entry; as the algorithm is based on determining the
 divisibility of g1 based on created an alternating pattern, the
 previous nonzero term is implied. 
 
 tldr; this only determines a subset of valid polygons
(c) Molly Feldman Summer@ICERM 2014. All Rights Reserved. 
[isPeriodic() and hasPeriod() courtesy of Michael Mossinghoff] 
*/

#include <iostream>
#include <istream>
#include <string>
#include <ostream>
#include <sstream>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>

using namespace std;

const int p = 3; /*prime number 1 */
const int q = 7; /*prime number 2 */
const int l = 5; /* prime numer 3 */

const int maxf2 = p*q;
const int maxf3 = l*p;

const int n = p*l*q;
int g1[n], g2[n], g3[n], f2[p*q], f3[p*l], F[n], inter[n];

int prev1 = 0; //previous nonzero values for each q*l segment
int prev2 = 0;
int prev3 = 0;

int result = 0;

/*coefficient vectors, representing how we can generate the coefficients given our algorithms*/

bool isPeriodic(int*, int);
bool hasPeriod(int*, int, int);
void setCoeffs();
void repeat(int*, int*, int, int, int, int);
int baseCases();
bool oddNumber(int*, int);
bool checkAlt(int*);
void set(int,int,int,int);
void produceG1(int); 
void generateGapsFinal(int*, int);
bool compareWithLargest(int, int, int, int, int, vector<int>);

////////////////////////////////////////////////////////////////////////
/* setCoeffs - basic "zeroing out" for all coefficient vectors use within */
void setCoeffs() {
	for (int i=0; i<n; i++) g2[i] = g3[i] = inter[i] =0;
	for (int j=0; j<n; j++) g1[j] = F[j] = 0;
	for (int e=0; e<p*q; e++) f2[e] = -1;
	for (int f=0; f<p*l; f++) f3[f] = -1;
}

///////////////////////////////////////////////////////////////////////////
/* repeat - repeats the coefficient sequence of v, with alternating sign,
and sets them to be the coefficient sequence of w using recursive definition.
used within to ensure the divisibility of g2, g3, and g1.
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
/* set- routine which sets the values for g1 and F based off of
		  the values determined in produceG1
		  readability routine */
void set(int i, int iterm, int aterm, int bterm) {
	g1[i] = iterm; F[i] = g1[i] + g2[i] + g3[i];
	g1[i+(q*l)]= aterm; F[i+(p*l)] = g1[i+(q*l)] + g2[i+(q*l)] + g3[i+(q*l)];
	g1[i+(2*q*l)] = bterm; F[i+(2*q*l)] = g1[i+(2*q*l)] + g2[i+(2*q*l)] + g3[i+(2*q*l)];
}

//////////////////////////////////////////////////////////////////////////
/* baseCases - determines (a) whether or not the given f2/f3 pass the
		necessary checks to create a valid F (namely that the coefficients in
		F are between 1 and -1 inclusive and (b) if valid, sets the values
		for 0, q*l, and 2*q*l as well as the previous value markers.
*/
int baseCases() { //sets up holdfast, required values

	//we know that F[0] == 1, as that's a necessary property
	if (inter[0] == -1 || inter[0] == -2) return 0;


	if (inter[0] == 0)  { 
		if (inter[q*l] == -2 || inter[2*q*l] == 2 || inter[2*q*l] == 1 || inter[q*l] == -1) return 0; 
		prev1 = 1;
		prev2 = -1;
		prev3 = 1;
		set(0, 1, -1, 1);
	}
	

	else if (inter[0] == 1) { 
		if (inter[q*l] == -2 || inter[q*l] == 2 || inter[2*q*l] == -2 || inter[2*q*l] == 2) return 0; //0 gives us a bad F
		prev1 = 1; //these are based off of inter[0] values and are "guesses" but are 
		prev2 = -1; //potentially valid
		prev3 = 1;
		set(0, 0, 0, 0);
	}
	else if (inter[0] == 2) { 

		if (inter[q*l] == 2 || inter[2*q*l] == -2 || inter[q*l] == 1 || inter[2*q*l] == -1) return 0;
		prev1 = -1;
		prev2 = 1;
		prev3 = -1;
		set(0, -1, 1, -1);
	}
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
/* reset- routine used to "zero out" values in the middle of recursive iterations; 
			  fallback test option to make sure that previous results don't effect
			  the global variables used in the recursive processes
*/
void reset(int i) {
	g1[i] = 0;
	g1[i+(q*l)] = 0;
	g1[i+(2*q*l)] = 0;
	F[i] = 0;
	F[i+(q*l)] = 0;
	F[i+(2*q*l)] = 0;

}

/////////////////////////////////////////////////////////////////////////
/* produceG1 - given a g2 and g3 determined in the main function, this function
	determines all possible valid associated g1 and then specifies the related 
	values for F. The function then checks to see if the resultant F is not
	Periodic, alternates in sign, and has the proper number of nonzero terms.
	Implemented recursively. 
	*/
void produceG1(int i) {
	//when i=q*l, we have gone through the entirety of F
	if (i==(q*l)) {
		if (!isPeriodic(F,n) && oddNumber(F,n) && checkAlt(F)) {
			//print out the associated normalized gap sequence
			generateGapsFinal(F,n); 
		}
		return;
	}
	//based on the value of inter, the sum of g1 and g2, it determines
	//all possible valid values for F and g1 the uses set to determine
	//the values and then stores the previous values for future 
	//iterations
	
	if (inter[i] == 2) {
		//only one case: can only be -1
		//(-1, 1, -1)
		if ((prev1 != 1) && (inter[i+(q*l)] <= 0) && (inter[i+(2*q*l)] >= 0) &&
			 (prev2 != inter[i+(q*l)] +1) && (prev3 != inter[i+(2*q*l)] - 1)) {
			 
			set(i,-1,1,-1);
			int val1[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] + 1 != 0) prev2 = inter[i+(q*l)] +1;
			if (inter[i+(2*q*l)] - 1 != 0) prev3 = inter[i+(2*q*l)] - 1;
			prev1 = 1;
			produceG1(i+1);
			prev1 = val1[0];
			prev2 = val1[1]; 
			prev3 = val1[2];
			 reset(i);	
		}
		//if invalid, simply returns back up the recursive chain
		reset(i);
		return;	
	}
	
	else if (inter[i] == -2) {
		//only one case: can only be 1
		//(1, -1, 1)
		if ((prev1 != -1) && (inter[i+(q*l)] >= 0) && (inter[i+(2*q*l)] <= 0) && 
			(prev2 != inter[i+(q*l)] - 1) && (prev3 != inter[i+(2*q*l)]+1)) {
			
			set(i,1,-1,1);
			int val2[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] - 1 != 0) prev2 = inter[i+(q*l)] -1;
			if (inter[i+(2*q*l)] + 1 != 0) prev3 = inter[i+(2*q*l)] + 1;
			prev1 = -1; 
			produceG1(i+1);
			prev1 = val2[0]; 
			prev2 = val2[1]; 
			prev3 = val2[2];
			reset(i);
		}
		reset(i);
		return;		
	}

	else if (inter[i] == 1) {
		//two cases: can only be -1 or 0
		//(0, 0, 0)
		if ((prev1 != 1) && (inter[i+(q*l)] != 2) && (inter[i+(q*l)] != -2) && 
			(inter[i+(2*q*l)] != 2) && (inter[i+(2*q*l)] != -2) && (prev2 != inter[i+(q*l)])
			&& (prev3 != inter[i+(2*q*l)])) {
			
			set(i,0,0,0);
			int val3[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] != 0) prev2 = inter[i+(q*l)] ;
			if (inter[i+(2*q*l)] != 0) prev3 = inter[i+(2*q*l)];
			prev1 = 1;
			produceG1(i+1);
			prev1 = val3[0];
			prev2 = val3[1]; 
			prev3 = val3[2];
			reset(i);
		}

		//(-1, 1, -1)
		if ((inter[i+(q*l)] <= 0) && (inter[i+(2*q*l)] >= 0) && (prev2 != inter[i+(q*l)] +1) && 
			(prev3 != inter[i+(2*q*l)] - 1)) {
			
			set(i,-1,1,-1);
			int val4[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] + 1 != 0) prev2 = inter[i+(q*l)] +1;
			if (inter[i+(2*q*l)] - 1 != 0) prev3 = inter[i+(2*q*l)] - 1;
			 produceG1(i+1);
			 prev1 = val4[0];
			 prev2 = val4[1];
			 prev3 = val4[2];
			 reset(i);	
		}
		reset(i);
		return;
	}

	else if (inter[i] == -1) {
		//only two cases: can either be 1 or 0

		// (0,0,0)
		if ((prev1 != -1) && (inter[i+(q*l)] != 2) && (inter[i+(q*l)] != -2) && 
		(inter[i+(2*q*l)] != 2) && (inter[i+(2*q*l)] != -2) && (prev2 != inter[i+(q*l)])
			&& (prev3 != inter[i+(2*q*l)])) {
			
			set(i,0,0,0);
			int val5[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] != 0) prev2 = inter[i+(q*l)] ;
			if (inter[i+(2*q*l)] != 0) prev3 = inter[i+(2*q*l)];
			prev1 = -1; 
			produceG1(i+1);
			prev1 = val5[0];
			prev2 = val5[1];
			prev3 = val5[2]; 
			reset(i);
		}

		//(1, -1, 1)
		if ((inter[i+(q*l)] >= 0) && (inter[i+(2*q*l)] <= 0) && (prev2 != inter[i+(q*l)] - 1) &&
		 (prev3 != inter[i+(2*q*l)]+1)) {
		 
			set(i,1,-1,1);
			int val6[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] - 1 != 0) prev2 = inter[i+(q*l)] -1;
			if (inter[i+(2*q*l)] + 1 != 0) prev3 = inter[i+(2*q*l)] + 1;
			produceG1(i+1);
			prev1 = val6[0];
			prev2 = val6[1];
			prev3 = val6[2];
			reset(i);
		}
		reset(i);
		return;
	}

	else if (inter[i] == 0) {
		
		//(1, -1, 1)
		if ((prev1 != 1) && (inter[i+(q*l)] >= 0) && (inter[i+(2*q*l)] <= 0) && 
			(prev2 != inter[i+(q*l)] - 1) && (prev3 != inter[i+(2*q*l)]+1)) {
			
			set(i,1,-1,1);
			int val7[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] - 1 != 0) prev2 = inter[i+(q*l)] -1;
			if (inter[i+(2*q*l)] + 1 != 0) prev3 = inter[i+(2*q*l)] + 1;
			prev1 = 1; 
			produceG1(i+1);
			prev1 = val7[0];
			prev2 = val7[1]; 
			prev3 = val7[2];
			reset(i);
		}

		//(-1, 1, -1)
		if ((prev1 != -1) && (inter[i+(q*l)] <= 0) && (inter[i+(2*q*l)] >= 0) && 
			(prev2 != inter[i+(q*l)] +1) && (prev3 != inter[i+(2*q*l)] - 1)) {
			
			set(i,-1,1,-1);
			int val8[] = {prev1, prev2, prev3};
			if (inter[i+(q*l)] + 1 != 0) prev2 = inter[i+(q*l)] +1;
			if (inter[i+(2*q*l)] - 1 != 0) prev3 = inter[i+(2*q*l)] - 1;
			 prev1 = -1; 
			 produceG1(i+1);
			 prev1 = val8[0];
			 prev2 = val8[1];
			 prev3 = val8[2];
			 reset(i);	
		}

		// (0,0,0)
		if ((inter[i+(q*l)] != 2) && (inter[i+(q*l)] != -2) && (inter[i+(2*q*l)] != 2) && 
			(inter[i+(2*q*l)] != -2) && (prev2 != inter[i+(q*l)]) && (prev3 != inter[i+(2*q*l)])) {
	
			set(i,0,0,0);
			int val9[]= {prev1, prev2, prev3};
			if (inter[i+(q*l)] != 0) prev2 = inter[i+(q*l)] ;
			if (inter[i+(2*q*l)] != 0) prev3 = inter[i+(q*l)];
			produceG1(i+1);
			prev1 = val9[0];
			prev2 = val9[1]; 
			prev3 = val9[2];
			reset(i);
		}
		reset(i);
		return;
	}
}

//////////////////////////////////////////////////////
/* oddNumber - checks that the last element in the 
	polygon is a +1; as F[0] = 1 as specified this
	is a requirement. */
bool oddNumber(int* v, int n) {
	int i = n-1;
	while (i>0){
		if (v[i] == 0) continue;
		else if (v[i] == -1) return false;
		else if (v[i] == 1) return true;
	}
	return false; 
}

////////////////////////////////////////////////////////////////
/* checkAlit = this function determines whether or not
	the nonzero terms in F alternate in sign, a requirement
	for Reinhardt polynomials. if not, it returns that the
	polynomial is invalid. */
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
/* main method - goes through all possible f2/f3 and then
runs the above methods to determine any valid g1 */
int main() {

	//sets up the coefficient vectors
	setCoeffs();

	char val1[p*q];
    
    for (int i=0; i<p*q; i++){
    	for (int j=0; j<(p*q)-i; j++) {
    	
    		int count1 = 0;
    		int k = (p*q) - i - j;
    		for (int r=0; r<i; r++) {
    			val1[count1] = '0';
    			count1++;
    		}
    		for (int q=0; q < j; q++) {
    			val1[count1] = '1';
    			count1++;
    			}
    		for (int s=0; s<k; s++) {
    			val1[count1] = '2';
    			count1++;
    		}
    
    		int len1 = strlen(val1);
			sort(val1, val1+len1);
			do {
    			int curr1[len1];
    			for (int i=0; i<len1; i++) {
    				if (val1[i] == '0') curr1[i] = 0;
    				else if (val1[i] == '2') curr1[i] = -1;
    				else if (val1[i] == '1') curr1[i] = 1;
    			}
    	
    			repeat(curr1, g2, maxf2, n, 1, 0);
    	
    			char val[p*l];
    
  	    		for (int i=0; i<p*l; i++){
    				for (int j=0; j<(p*l)-i; j++) {
    	
    					int count = 0;
    					int k = (p*l) - i - j;
    					for (int r=0; r<i; r++) {
    						val[count] = '0';
    						count++;
    					}
    					for (int q=0; q < j; q++) {
    						val[count] = '1';
    						count++;
    					}
    					for (int s=0; s<k; s++) {
    						val[count] = '2';
    						count++;
    					}
    	
    					int len = strlen(val);
						sort(val, val+len);
						do {
    						int curr[len];
    						for (int i=0; i<len; i++) {
    							if (val[i] == '0') curr[i] = 0;
    							else if (val[i] == '2') curr[i] = -1;
    							else if (val[i] == '1') curr[i] = 1;
    						}
    		
							repeat(curr, g3, maxf3, n, 1, 0);
			
							for (int i=0; i<n; i++) {
								inter[i] = g2[i] + g3[i];
							}
						
							baseCases();
							produceG1(1);
    		
				  		} while (next_permutation(val, val+len));	
					}
				}    	
			} while (next_permutation(val1, val1+len1));
		}
	}
	cout << "all done" << endl; 
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
/* generateGapsFinal - takes in a coefficient array and then produces the 
normalized gap sequence associated with that coefficient array for determining the
associated polygon. uses the known normalization rule.
*/

void generateGapsFinal(int* v, int n) { 
	//first need to generate the gap sequence and store it
	vector<int> gaps;	

	int newv[n+1];
    for (int i=0; i<n; i++) newv[i] = v[i];
   	newv[n] = -1;
	 
	int k=-1, gap;
	while (++k < n) {
		gap = 0;
		for (gap=0; k < n && newv[k] == 0; k++) gap++;
		gaps.push_back(gap+1);
	}
	
	//finished setting up gap sequence

	//finds the largest value in the gap array
	int large = 0;
	for (int h=0; h<gaps.size(); h++) {  
		if (gaps[h] > large) {
			large  = gaps[h];
		}
	}

	//sets location as a vector containing all the locations in gaps
	//where the largest value can be found
	vector<int> location;
	for (int l=0; l<gaps.size(); l++) {
		if (gaps[l] == large) {
			location.push_back(l);	
		}
	}

	
	//winner contains the location of the largest value and the direction
	//that represents the correct normalized orientation of the gap sequence
	int winner[2];
	winner[0] = location[0]; 
	winner[1] = 1;

	//goes through all largest values and compares their left and right
 	//sequences to one another, determines a winner, and then compares
  	//the "internal" winner to the overall current winner
	for (int i=0; i<location.size(); i++) {
		int size = gaps.size();
		int sum_left = large;
		int sum_right = large;
		int val = location[i];
		int val2 = location[i];

		while (true) {
			
			val++;
			val2--;

			if (val > size-1) {
				val = 0;
			}
			if (val2 < 0) {
				val2 = size-1;
			}
			
			if (gaps[val] > gaps[val2]) { //so the right scan is better
				//now you need to compare it with the current favorite
				if (compareWithLargest(large, winner[0], winner[1], location[i], 1, gaps)) {
					winner[0] = location[i];
					winner[1] = 1;
				}
				break;

			}
			else if (gaps[val2] > gaps[val]) { //so the left scan is better
				if (compareWithLargest(large, winner[0], winner[1], location[i], -1, gaps)) {

					winner[0] = location[i];
					winner[1] = -1;
				}
				break;
			}
			//NEED TO CHANGE THIS TO ALIGN WITH WHAT IS IN NORMALTRY [7/31/14]
			else if (val == location[i]) {
				if (compareWithLargest(large, winner[0], winner[1], location[i], 1, gaps)) {

					winner[0] = location[i];
					winner[1] = 1;
				}
				break;
			}
			else if (val2 == location[i]) {
				if (compareWithLargest(large, winner[0], winner[1], location[i], -1, gaps)) {

					winner[0] = location[i];
					winner[1] = -1;
				}
				break;
			}
		}	
	}
		
	vector<int> meantime;
	meantime.push_back(gaps[winner[0]]);
	
	if (winner[1] == 1) { //if we are going right
		for (int k= winner[0] +1; k<gaps.size(); k++) {
			meantime.push_back(gaps[k]);
		}
		for (int j=0; j < winner[0]; j++) {
			meantime.push_back(gaps[j]);
		}
	}
	

	if (winner[1] == -1) { //if we are going left
		for (int i = winner[0] -1; i>= 0; i--) {
			meantime.push_back(gaps[i]);
		}
		for (int j = gaps.size()-1; j > winner[0]; j--) {
			meantime.push_back(gaps[j]);
		}
	}
	for (int i=0; i< meantime.size(); i++) {
		cout << meantime[i] << " ";
	}
	cout << endl;

}

/////////////////////////////////////////////////////////////////////////////////
/* compareWithLargest - a helper function to generateGapsFinal
	which compares the current overall winner with the winner from the
	current location iteration. similar mechanism to generateGapsFinal
	*/
bool compareWithLargest(int large, int largeloc, int largedir, int comploc, int compdir, vector<int> gaps) {
		

		//right is 1, left is -1
		int size = gaps.size();
		int val = largeloc;
		int val2 = comploc;
		bool ret = false;

		while (true) {
			
			val = val + largedir;
			val2 = val2 + compdir;

			if (val > size-1) {
				val = 0;
			}
			else if (val < 0) {
				val = size-1;
			}

			if (val2 > size-1) {
				val2 = 0;
			}

			else if (val2 < 0) {
				val2 = size-1;
			}

			if (gaps[val] > gaps[val2]) { //the current largest wins
				break;
			}
			else if (gaps[val2] > gaps[val]) { //the comparison wins
				ret = true;
				break;

			}
			else if (val == largeloc || val2 == comploc) {
				ret = true;
				break;
			}
		}
		return ret;
}

/////////////////////////////////////////////////////////////////////////////////////
/* isPeriodic - returns whether an inputed coefficient array represents
		a periodic polygon; namely it determines whether or not  there is a 
		discernable pattern in the coefficients. */
bool isPeriodic(int* c, int n) {
    for (int t=2; t<=n/2; t++) {
        if (hasPeriod(c, n, t)) return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////
/* hasPeriodic - helper function for isPeriodic */
bool hasPeriod(int* c, int n, int t) {
    for (int i=0; i<n-t; i++) {
        if (c[i] != -c[i+t]) return false;
    }
    return true;
}