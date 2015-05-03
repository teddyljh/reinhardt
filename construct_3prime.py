#This program constructs a 3-prime times r Reinhardt polygon

import numpy as np
import random

p = 3
q = 5
l = 7
r = 3
N = p*q*l*r

pq = p*q
pl = p*l
ql = q*l
pql = p*q*l

perm_set = []  #set used for getting Se and So 

rn_set = [] #set for compositions of r

#build F for the given N 

def findF(rn, outfile):
    
    A = np.zeros((pq, r), np.int32)
    B = np.zeros((pl, r), np.int32)
    C = np.zeros((ql, r), np.int32)
    
    g1= np.zeros(N, np.int32)
    g2 = np.zeros(N, np.int32)
    g3 = np.zeros(N, np.int32)
    
    g12 = np.zeros(N, np.int32) #g1+g2
    
    f1 = np.zeros(N/l, np.int32)
    f2 = np.zeros(N/q, np.int32)
    f3 = np.zeros(N/p, np.int32)
    
    #f = np.zeros((pql, n), np.int32) #2-dim f divided into 105 blocks
    F = np.zeros(N, np.int32) #1-dim final F
    
    #get g1
    for i in range(1, pq+1):
             
        A[i-1] =findA(rn, i) 
    
    f1 = reduce_dim(A)   
    g1 = repeat(f1, l)
    
    #print "f1:", f1
    
    #get g2
    for i in range(1, pl+1):
        B[i-1] = findB(rn, i)
    
    f2 = reduce_dim(B)    
    g2 = repeat(f2, q)
    
    #print "f2:",f2
    
    #print 'g1:',g1
    #print 'g2:', g2
    
    for i in range(N):
            g12[i] = g1[i] + g2[i]
    
    #print 'g12:', g12          
                    
    #select the Ci for i from 2 to ql-1      
    for i in range(1, ql):
        C[i-1] = findC(rn, i)
    
    C[ql-1] = findC(rn, ql)    
    
    #print "C:", C
    
    f3 = reduce_dim(C)
    
    #print "f3:", f3                       
    
    g3 = repeat(r_operator(f3), p)
    
    #print "g3:", g3
    
    for i in range(N):
            F[i] = g12[i] + g3[i]
            
    #print "F:", F
    
    print check_valid(F),
    print check_alt(F)
    

    if check_valid(F) == True and check_alt(F) == True and check_period(print_gaps(F)) == False:
        #print >>outfile, "g1:", convert_sign(g1)
        #print >>outfile, "g2:", convert_sign(g2)
        #print >>outfile, "g3:", convert_sign(g3)
    
        #print >>outfile, "F:", convert_sign(F)
        #print >>outfile, print_gaps(F)
    
        #print "g1:", convert_sign(g1)
        #print "g2:", convert_sign(g2)
        #print "g3:", convert_sign(g3)
        print "F:", convert_sign(F)
        
        #print print_gaps(F)
        
         
#find the A  
#m = 0 or 1, j goes from 0 to 2
def findA(rn, i):
    a = []
    n = len(rn)
    
    for j in range(n):
        if j == 0:
            aijset = get_so(rn[j]+1, (-1)**(i+1))
            
            if i == 1:
                startw1 = []
                for item in aijset:
                    if item[0] == 1:
                         startw1.append(item)
                         #aij = item
                         
                m = len(startw1)
                aij = startw1[random.randint(0,m-1)] 
            
                for num in aij:
                    a.append(num)
                
            else:
                m = len(aijset)    
                #print "Aij set:", aijset
                aij = aijset[random.randint(0,m-1)] 
            
                for num in aij:
                    a.append(num)
                
                
        elif (j+1)%3 == 1:  #this part is in fact not executed for n=3
            aijset = get_se(rn[j]+1, (-1)**i)  
            #print "Aij set:", aijset
            aij = aijset[m]
            for num in aij:
                a.append(num)
                    
        elif (j+1)%3 == 0:
            for num in range(rn[j]-1):
                a.append(0)
                    
        elif (j+1)%3 == 2:
            for num in range(rn[j]):
                a.append(0)
                    
    return a           
    
    
#find Bi   
def findB(rn, i):
    b = []
    n = len(rn)
    
    for j in range(n):
        
        if (j+1)%3 ==2:
            bijset = get_se(rn[j]+1, (-1)**i)
            m = len(bijset)
            bij = bijset[random.randint(0,m-1)]
            for num in bij:
                b.append(num)
                
        elif (j+1)%3 == 0:
            for num in range(rn[j]-1):
                b.append(0)
            
        elif (j+1)%3 == 1:
            for num in range(rn[j]):
                b.append(0)

    return b
    

#find Ci

def findC(rn, i): 
    c = []
    n = len(rn)
    
    for j in range(n):
            if (j+1)%3 == 0:
                cijset = get_se(rn[j]+1, (-1)**i)
                
                if i == ql:
                    endw0 = []
                    for item in cijset:
                        if item[len(item)-1] == 0:
                            endw0.append(item)
                         
                    m = len(endw0)
                    cij = endw0[random.randint(0,m-1)] 
            
                    for num in cij:
                        c.append(num)
                        
                else:
                    m = len(cijset)
                    cij = cijset[random.randint(0,m-1)]

                    for num in cij:
                        c.append(num)
            
            elif (j+1)%3 == 1:
                for num in range(rn[j]-1):
                    c.append(0)
            
            elif (j+1)%3 == 2:
               for num in range(rn[j]):
                    c.append(0)
        
    return c



#the Se operator
#generate a k-length array that contains even-number alternating 1,-1
#and start with b

def get_se(k,b):
    seq = []
    
    #array of all zeros is included regardless of b
    
    #generate an array with length k-1 and odd number of nonzeros
    #then add b to the first during permutation
    i = 0
    while i<=k:
        se = []
        for j in range(i):
                se.append(1)
        for j in range(i,k):
                se.append(0)
        
        #print se
        permutate(se, 0, k, b)
        i+=2
  
    for array in perm_set:
           seq.append(array)
    
    #clear perm_set
    perm_set[:] = []
    
    return seq

#the So operator
#generate a k-length array that contains odd-number alternating 1,-1
#and start with b

def get_so(k,b):
    seq = []
  
    #generate an array with length k-1 and even number of nonzeros
    #then add b to the first during permutation
    i = 1
    while i<=k:
        so = []
        for j in range(i):
                so.append(1)
        for j in range(i,k):
                so.append(0)
        #print so
        permutate(so, 0, k, b)
        
        i+=2
  
    for array in perm_set:
           seq.append(array)
    
    #clear perm_set
    perm_set[:] = []
    
    return seq


#generate a permutation
def permutate(array, t, n, b):
    nonzero = 0   
     
    perm = []
    
    if t == n:
        if b != 1 and b != -1:
            for i in range(n):
                perm.append(array[i])
        
            rn_set.append(perm)
            return
        
        else:            
                
            for i in range(n):
                #print array[i]
                if array[i] == 1 and nonzero == 0:
                    perm.append(b)
                    nonzero+=1
                    
                elif array[i] == 1 and nonzero%2 == 1:
                    perm.append(-array[i]/b)
                    nonzero+=1
                
                elif array[i] == 1 and nonzero%2 == 0:
                    #print array[i]/b
                    perm.append(array[i]/b)
                    nonzero+=1
                    
                else:
                    #print 0
                    perm.append(0)
                
            perm_set.append(perm)
            return
    
    for j in range(t,n):
        flag = 1
        for r in range(t,j):
            if array[r] == array[j]:
                flag = 0
                break
            
        if flag == 0:
            continue
        else:
            array[j],array[t] = array[t],array[j]
            permutate(array,t+1,n,b)
            array[j],array[t] = array[t],array[j]           

#n is the integer to partition, k is the length of partitions
#l is the min partition element size

def partition(n,k,l=1):
    
    if k < 1:
        raise StopIteration
    if k == 1:
        if n >= l:
            yield (n,)
        raise StopIteration
    for i in range(l,n+1):
        for result in partition(n-i,k-1,i):
            yield (i,)+result

                
def check_valid(array):
    for num in array:
        if num >= 2 or num <= -2:
            return False
    
    return True

def check_alt(array):
    sign = array[0]
    n = len(array)
    
    if sign != 1:
        return False
    
    for i in range(1,n):
        if array[i] == 0:
            continue
        if array[i] == sign:
            return False
        if array[i] == -sign:
            sign = -sign
    
    if sign == -1:
        return False
        
    return True
    
#repeat and alternate 1-dim array
def repeat(array, n):
    poly = []
    for i in range(n):
        if i%2 == 0:
            for item in array:
                poly.append(item)
        else:
            for item in array:
                poly.append(-item)
                
    return poly
       
#the R operator: shift-right-and-negate
def r_operator(array):                      
    n = len(array)
    new = []
    
    neg_last = -array[n-1]
        
    new.append(neg_last)
    for i in range(n-1):
        new.append(array[i])
    
    return new   
                        
def print_gaps(array):
    zero = 0
    poly = []
    n = len(array)
    for i in range(1,n): 
        if array[i] == 0 or array[i] == '0':
            zero+=1
        elif array[i] == 1 or array[i] == '+' or array[i] == -1 or array[i] == '-':
            #print zero+1,
            poly.append(zero+1)
            zero = 0
    #print zero+1
    poly.append(zero+1)
    
    #print sum(poly)
    
    return poly   

#convert integers into signs
def convert_sign(array):
    n = len(array)
    poly = "" 
    i = 0
    for i in range(n):
        if array[i] == 1:
            #print '+',
            poly = poly + '+'
            
        elif array[i] == -1:
            #print '-',
            poly = poly + '-'
        else:
            #print 0,
            poly = poly + '0'
            
    return poly
    
#reduce a  2-dim array to 1-dim
    
def reduce_dim(array):
    n = len(array)
    poly = []
    
    for i in range(n):
        for item in array[i]:
            poly.append(item)

    return poly

#check if the polynomial is periodic
def check_period(array):
    n = len(array)
    for i in range(1, n/2+1):
        if has_period(array, i) ==  True:
            return True
            
    return False

def has_period(array, t):
    n = len(array)
    for i in range(0, n-t):
        if array[i] != array[i+t]:
            return False
    
    return True
    
            
def main():
    #get the composition of r
    n = 3
    for result in partition(r, n):
        part = list(result) #convert tuple tp list
        
        permutate(part, 0, n,  0)
        
    #print rn_set
            
    out = open("data420.txt", "w")
    
    for rn in rn_set:
        for i in range(1000):
            #print rn
            findF(rn,out)
            #print findA(rn, 1)

                                 
    out.close()
    
main()