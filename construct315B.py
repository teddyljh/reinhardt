#This program constructs a n=315 Reinhardt polygon
#which permutates all possible g2 with randomization of g1 and g3

import random


p = 3
q = 5
l = 7
r = 3
N = p*q*l*r

pq = p*q
pl = p*l
ql = q*l

perm_set = []   

perm_b = [] 


#build F for the given N 

def findF(rn, b_list, outfile):
    n = len(rn)
    
    A = []
    B = []
    C = []
    
    g1= []
    g2 = []
    g3 = []
    g12 = [] #g1+g2

    #f1 = []
    #f2 = []
    #f3 = []

    f = [] #2-dim f divided into 105 blocks
    F =[] #1-dim final F
    
    for i in range(1, pq+1):
             
        #se = [[b,0],[0,b]], so = [[0,0],[b,-b]]
            
        #m = 0: i odd: [1,0,0], i even: [-1, 0, 0]
        #m = 1: i odd :[0,1,0],i even: [0,-1,0]
        A.append(findA(rn, i, random.randint(0,1)))  
        
    g1 = repeat(A, l)
    
    for i in range(1, pl+1):
        B.append(findB(rn, i, b_list[i-1]))
        
    g2 =  repeat(B, q)
    
    #print 'g1:',g1
    #print 'g2:', g2
    
    for i in range(N/n):
            g12.append(add(g1[i], g2[i]))
    
    #print 'g12:', g12          
    
    #find C[0]
    #make sure F starts with 1
        
    if g12[0][n-1] != 0 or g12[1][0] == 1:
            C.append(findC(rn, 1, 0, 0))
    else:
            C.append(findC(rn, 1, random.randint(0,1), 0))
    
    
    #select the Ci for i from 2 to ql-1      
    for i in range(2, ql+1):
        C.append(findC(rn, i, random.randint(0,1), C[i-2][n-1]))
    
    
    #print "C:", C
    
    g3 = repeat(C, p)
    #print "g3:", g3
    
    for i in range(N/n):
        f.append(add(g12[i], g3[i]))
    
    #print "f:", f
    
    F = reduce_dim(f)
    
    #print check_valid(F),
    #print check_alt(F)
    
    #print "F:", F
    
    if check_valid(F) == True and check_alt(F) == True and check_period(print_gaps(F)) == False: 
        #print >>outfile, "g1:", convert_sign(reduce_dim(g1))
        #print >>outfile, "g2:", convert_sign(reduce_dim(g2))
        #print >>outfile, "g3:", convert_sign(reduce_dim(g3))
    
        print >>outfile, "F:", convert_sign(F)
        print >>outfile, print_gaps(F)


         
#find the A  
#m = 0 or 1, j goes from 0 to 2
def findA(rn, i, m):
    a = []
    n = len(rn)
    
    aijset = []
    for j in range(n):
        if i == 1:
            a = [1,0,0]
            
        elif j == 0:
            aijset = get_so(rn[j]+1, (-1)**(i+1))
                    
            #print "Aij set:", aijset
            aij = aijset[m] 
            
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
def findB(rn, i, m):
    b = []
    n = len(rn)
    
    for j in range(n):
        if (j+1)%3 ==2:
            bijset = get_se(rn[j]+1, (-1)**i)
            bij = bijset[m]
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
#gives a Ci that has already been R-operated
#only works for this case
#j is not needed here

def findC(rn, i, m, former): 
    c = []
    
    if i%35 == 1:
        c.append(0)
    else:
        c.append(-former)
    
    c.append(0)
    
    if m == 0 or i%35 == 0:
        c.append(0)

    elif m == 1:
        c.append((-1)**i)
        
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
        if b != 1 and b != -1:#used to permutate the f set
            for i in range(n):
                perm.append(array[i])
        
            perm_b.append(perm)
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

#repeat a,b,c to get g1,g2,g3
#alternate the repeating sign
#NOTE: works for 2 dim array

def repeat(array, n):
    poly = []
    for i in range(n):
        if i%2 == 0:
            for item in array:
                poly.append(item)
        else:
            for item in array:
                poly.append(negate(item))
    
    return poly
    
#negate all elements in an array
def negate(array):
    poly = []
    for num in array:
        poly.append(-num)
    
    return poly
    
#add the elements of two 2-dim arrays
def add(a,b):
    s = []
    n = len(a)
    for i in range(n):
        s.append(a[i]+b[i])
            
    return s     
        
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
    
    rn = [1,1,1]
    
    a_list = [0]*pl
    for i in range (pl):
        for j in range(i):
            a_list[j] = 1
        
        permutate(a_list, 0, pl, 0)
    
    #print len(perm_a)
    #print perm_a
    
       
    out = open('data315_B7.txt', 'w')

    #for i in range(len(perm_b)):
    for i in range(600000,700000):
        findF(rn, perm_b[i], out)
       
    out.close()
    
main()
    