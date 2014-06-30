//  main.cpp
//  polyGen_alt
//
//  Created by Teddy Liu on 6/27/14.
//  Copyright (c) 2014 Reinhardt Group. All rights reserved.
//

/*This code generates arrays of length n that starts with 1, followed by
alternating 1, -1 and 0 in between.
 The idea is that first generates arrays of length (n-1) that only contains 
 1 ans 0. Then do a full permutation of each array.
 When printing, print 1 first, then count the position of 1 to decide whether 
 there should be a -1 at this position and print out.
 */


#include <iostream>
using namespace std;

const int n = 5;
int polyset[n][n-1]; //a 2-dim array to put in combination of 0 and 1

void combination(int n)
{
    //first generate an array of only 1 and 0 of length n-1
    int count = 0;
    for (int m = 0;m<n;m++)//the total number of 1 (>1 and <=n since all 0 is invalid)
    {
        for(int i = 0;i<m;i++)
        {
            polyset[count][i] = 1;
        }
        
        //print out the combinations
        for (int q=0; q<n-1;q++)
        {
            cout<<polyset[count][q];
        }
        
        count++;
        cout<<endl;
    }
    cout<<"Number of combination:"<<count<<endl;
    //the number of combination of 1 and 0 is always n
    
}


void enumerate(int a[], int t, int n)
{

    int nonzero = 0;
    //in order to deicide when to print 1 and when to print -1

    if(t == n)
    {
        cout << "in t == n statement " << endl;
        cout<<1<<" "; //the first element is always 1
        for(int i=0;i<n;i++)
        {
            if(a[i] == 1 && nonzero%2 == 1)
            {
                
                cout<<a[i]<<" "; //print 1
                nonzero++;
            }
            
            else if (a[i] == 1 && nonzero%2 == 0)
            {
                cout<<-a[i]<<" ";//print -1
                nonzero++;

            }
            
            else
            {
                cout<<a[i]<<" ";//print 0
            }
            
        }
        cout<<endl;
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
            enumerate(a,t+1,n);
            swap(a[j],a[t]);
        }
    }
}

int main(int argc, const char * argv[])
{
    combination(n);
    
    for (int i = 0; i<n; i++)
    {
        enumerate(polyset[i], 0, n-1);
    }
    
    
    return 0;
}

