#This file contains print and helper functions for constructing polygons
import numpy as np

def convert_num(poly):
    n = len(poly)
    array = []
    i = 0
    for i in range(n):
        if poly[i] == '+':
            array.append(1)
        elif poly[i] == '-':
            array.append(-1)
        else:
            array.append(0)
    print array
    return array
    
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

def print_gaps(array):
    zero = 0
    poly = []
    n = len(array)
    for i in range(1,n): 
        if array[i] == 0 or array[i] == '0':
            zero+=1
        elif array[i] == 1 or array[i] == '+' or array[i] == -1 or array[i] == '-':
            print zero+1,
            poly.append(zero+1)
            zero = 0
    print zero+1
    poly.append(zero+1)
    
    return poly

#generate all the shifted dihedrals of a [1,0,-1] array

def shift_dih(array):
    n = len(array)
    dih_set = []
    dih_set.append(array)
    
    for i in range(1,n):           
            if array[i] == 1:
                dih = []
                for num in array[i:n]:
                    dih.append(num)
                
                for num in array[0:i]:
                    dih.append(-num)
                print dih
                print check_alt(dih)
                dih_set.append(dih)



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

                
def test():
    #poly = "+-000+000000-+00-0000000+-000+00000000000-000000+-000+00-+00"
    
    #shift_dih(convert_num(poly))
    
    #poly2 = "+00-0+0-0+0-00000+0-0+0-0+00000-0+0-0+0-00"
    #print_gaps(poly2)
    
    #array = [1,0,-1,1,0]
    #print convert_sign(array)
    
    #in_data = open('polydata.txt','r')
    #out_data = open('poly_num.txt','r')
    
    

test()