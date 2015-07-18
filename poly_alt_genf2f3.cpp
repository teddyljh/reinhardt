//  main.cpp
//  polyGen_alt
//
//  Copyright (c) Teddy Liu, Summer@ICERM 2014. All rights reserved.
//

/*This code generates arrays of length n that starts with 1, followed by
alternating 1, -1 and 0 in between as coefficients of the polynomial for
 construction of a p*q*l*r polygon
 */


#include <iostream>
using namespace std;

const int p = 3;
const int q = 5;
const int l = 7;
const int r = 1;
const int n = p*q*l*r;

const int pl= p*l; //21
const int pq = p*q; //15

int polyf2[pl][pl-1]; //a 2-dim array to put in combination of 0 and 1
int polyf3[pq][pq-1];

/*capacity of the array that contains the permutation
 need to be adapted when p,q,l change*/
const int capf2 = 1048576;
const int capf3 = 16384;

int f2[capf2][pl]; //all permutation of f2 coeff
int f3[capf3][pq]; // all permutation of f3 coeff

int countf2 = 0;
int countf3 = 0;


//////////////////////////////////////
void comb(int);
void enumf(int*, int, int);



void comb(int n)
{
    if (n == pl)
    {//first generate an array of only 1 and 0 of length n-1
        int count = 0;
        for (int m = 0;m<n;m++)//the total number of 1 (>1 and <=n since all 0 is invalid)
        {
            for(int i = 0;i<m;i++)
            {
                polyf2[count][i] = 1;
            }
            
            //print out the combinations
            for (int q=0; q<n-1;q++)
            {
                cout<<polyf2[count][q];
            }
            
            count++;
            cout<<endl;
        }
    }
    
    else if (n == pq)
    {
        //first generate an array of only 1 and 0 of length n-1
        int count = 0;
        for (int m = 0;m<n;m++)//the total number of 1 (>1 and <=n since all 0 is invalid)
        {
            for(int i = 0;i<m;i++)
            {
                polyf3[count][i] = 1;
            }
            
            //print out the combinations
            for (int q=0; q<n-1;q++)
            {
                cout<<polyf3[count][q];
            }
            
            count++;
            cout<<endl;
        }
    }
}


void enumf(int a[], int t, int n)
{
    if(n == pl-1)
    {
        int nonzero = 0;
        //in order to deicide when to assign 1 and -1
        
        if(t == n)
        {
            cout<<"+"<<" ";
            f2[countf2][0] = 1;//the first element is always 1
            //cout<<f2[countf2][0];
            for(int i=0;i<n;i++)
            {
                if(a[i] == 1 && nonzero%2 == 1)
                {
                    
                    cout<<"+"<<" "; //print +
                    f2[countf2][i+1] = 1;
                    //cout<<f2[countf2][i+1];
                    nonzero++;
                }
                
                else if (a[i] == 1 && nonzero%2 == 0)
                {
                    cout<<"-"<<" ";//print -
                    f2[countf2][i+1] = -1;
                    //cout<<f2[countf2][i+1];
                    nonzero++;
                    
                }
                
                else
                {
                    cout<<a[i]<<" ";//print 0
                    f2[countf2][i+1] = 0;
                    //cout<<f2[countf2][i+1];
                }
                
            }
            cout<<endl;
            countf2++;
            return;
        }
        
        
        
        for(int j=t;j<n;j++)
        {
            int flag = 1;
            for(int r=t;r<j;r++)
            {
                if(a[r] == a[j])
                {
                    flag = 0;
                    break;
                }
            }
            
            if(flag == 0)
            {
                continue;
            }
            
            else
            {
                swap(a[j],a[t]);
                enumf(a,t+1,n);
                swap(a[j],a[t]);
            }
        }
    }
    
    else if (n == pq-1)
    {
        int nonzero = 0;
        //in order to deicide when to assign 1 and -1
        
        if(t == n)
        {
            cout<<"+"<<" ";
            f3[countf3][0] = 1;//the first element is always 1
            for(int i=0;i<n;i++)
            {
                if(a[i] == 1 && nonzero%2 == 1)
                {
                    
                    cout<<"+"<<" "; //print +
                    f3[countf3][i+1] = 1;
                    nonzero++;
                }
                
                else if (a[i] == 1 && nonzero%2 == 0)
                {
                    cout<<"-"<<" ";//print -
                    f3[countf3][i+1] = -1;
                    nonzero++;
                    
                }
                
                else
                {
                    cout<<a[i]<<" ";//print 0
                    f3[countf3][i+1] = 0;
                }
                
            }
            cout<<endl;
            countf3++;
            return;
        }
        
        for(int j=t;j<n;j++)
        {
            int flag = 1;
            for(int r=t;r<j;r++)
            {
                if(a[r] == a[j])
                {
                    flag = 0;
                    break;
                }
            }
            
            if(flag == 0)
            {
                continue;
            }
            
            else
            {
                swap(a[j],a[t]);
                enumf(a,t+1,n);
                swap(a[j],a[t]);
            }
        }
        
    }
    
    
}


int main(int argc, const char * argv[])
{
    
    //comb(pl);
    //comb(pq);
    
    
    for (int k=0; k<capf2; k++) {
    for (int i = 0; i<pl; i++)
    {
        cout << f2[k][i];
        //enumf(polyf2[i], 0, pl-1);
        
    }
        cout << endl;
    }
    //cout<<"Permutation of f2:"<<countf2<<endl;
    
    
    for (int j = 0; j<pq; j++)
    {
        //cout << polyf3[j] << endl;
        enumf(polyf3[j], 0, pq-1);
    }
    
    //cout<<"Permutation of f3:"<<countf3<<endl;
    
    //check the array
    for (int i = 0; i<1048576; i++)
     {
     for (int j = 0;j<pl; j++)
     {
     cout<<f3[i][j];
     }
     cout<<endl;
     }

    for (int i = 0; i<16384; i++)
    {
        for (int j = 0;j<pq; j++)
        {
            cout<<f3[i][j];
        }
        cout<<endl;
    }
    
    
    return 0;
}

