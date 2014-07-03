//pseudocode for bool array
//we have g2[] and g3[] already, we want to create b[][] which tells us which values of //g1[] would be possible and then print if it’s good
 

//bools go that b[0][i] == 1, b[1][i] == 0, b[2][i] == -1

bool b[3][35];
int val1 =0;
int val2 =0;
int val3 =0;
 
//BASIC BASE CASE CREATION OF THE ARRAY 
b[1][0]=false; //0
b[2][0]=false; //-1

b[1][35] = false; //0
b[0][35]=false; //1

b[1][70] = false; //0
b[2][70] = false; //-1
 
for(int i=1; i<35; i++) //constructs b (bool array)
{
      //values represent the sums of the terms at the three points of consideration
      val1 = g2[i]+g3[i];
      val2 = g2[i+35]+g3[i+35];
      val3 = g2[i+70]+g3[i+70];

      if (val1 or val2 or val3 = 2)
      {
                  b[1][i]=false
                  b[2][i]=false
                  if(i<34)
                  {
                              b[3][i+1]=false
                  }
      }
      if (val1 or val2 or val3 = 1)
      {
                  b[1][i]=false
      }
      if (val1 or val2 or val3 = -1)
      {
                  b[3][i]=false
      }
      if (val1 or val2 or val3 = -2)
      {
                  b[2][i]=false
                  b[3][i]=false
                  if(i<34)
                  {
                              b[1][i+1]=false
                  }
      }
}
for( j =0 to 34) //initializes g1 array
{
            if(b[1][i]=true)
            {
                        g1[i]=1
                        g1[i+35]=-1
                        g1[i+70]=1
            } else if(b[2][i]=true)
            {
                        g1[i]=0
                        g1[i+35]=0
                        g1[i+70]=0
            }else if (b[3][i]=true)
            {
                        g1[i]=-1
                        g1[i+35]=1
                        g1[i+70]=-1
            } else
            {
                        break //all false -> next iteration
            }
}
 
int k=0
int h=0
 
while(h<35) //goes through all possible iterations for g1 (using b)
{
            for(i =0 to 104) //checking alternating restriction
            {
                        k+=(g1[i]+g2[i]+g3[i])
                        if(k>1 or k<0)
                        {
                                    k=0
                                    break //could use a goto instead
                        }
            }
            if (k=1) //checking odd # of terms (as well as breaking caused by alternating)
            {
                        print(g1,g2,g3,F) //I think you have a print method already?**
            }
 
            while (h<35) //this finds next iteration of g1
            {
                        g1[h]++
                        while(b[(g1[h]+1)%3][h] is false)
                        {
                                    g1[h]++
                        }
                        if(g1[h]>1)
                        {
                                    g1[h]=(g1[h]+1)%3-1
                                    g1[h+35]=-1*g1[h]
                                    g1[h+70]=g1[h]
                                    h++
                        }else
                        {
                                    g1[h+35]=-1*g1[h]
                                    g1[h+70]=g1[h]
                                    break
                        }
            }
}