# -*- coding: utf-8 -*-
'''
addpolynomials.py
this program takes a generating set of known
n length poynomials as space separated gap sequences and returns all
valid Reinhardt n-gons, r, where r = (-/+) x (-/+) y (-/+) z and
x,y,z are elements of the generating set and for each possible combination
there are two + and one -. 
the results are returned as normalized gap sequences according to the
known convention. 
(c) Molly Feldman, Summer@ICERM, 2014
[isPeriodic() and hasPeriod() courtesy of Michael Mossinghoff]
'''


import numpy as np
import sys
n = int(sys.argv[2]) #number of sides of the polygons you are adding together

def main():

  filename = sys.argv[1] #file with basis
  dat = open(filename, 'r')
  data = dat.readlines()
  polys = []
  #gets gap sequences from the file
  for i in range(len(data)):
    new = data[i].lstrip()
    new = new.rstrip()
    new = new.split(' ')
    sum = 0
    allint = []
    #converts gap sequences to integer values, for
    #eventual comparison with addition results
    for j in range(len(new)):
    	#print j
    	allint.append(int(new[j]))
    	sum += int(new[j])
    polys.append(allint)
    #print sum
    

  plus = []
  #converts from gaps to coefficients
  for k in range(len(polys)): 
  	curr = []
  	curr.append(1)
  	prev = 1
  	
  	edit = polys[k]
  	for i in range(len(edit)):
  		for j in range(int(edit[i])-1):
  			curr.append(0)
  		
  		if (prev == 1):
  			curr.append(-1)
  			prev = -1
  		else:
  			curr.append(1)
  			prev = 1

  	plus.append(curr[:len(curr)-1])
  #plus contains all coefficient representations of basis polygons
  
  count = 0
  #all = []			#****uncomment lines referencing all to obtain
  						# results that do not include multiple copies of any polynomial
  						# ONLY WORKS FOR VERY SMALL n ******
  						
  #main loops; goes through all possible combinations of 3 polynomials
  for i in range(len(plus)):
  	for j in range( len(plus)):
  		for k in range(len(plus)):
  			elt = plus[i]
  			item = plus[j]
  			obj = plus[k]
  			
  			#uncomment the sys.stderr line to print out what addition
  			#step you are on (helpful error check) 
  			qq = ""
  			qq = str(i+1) + " " + str(j+1) + " " + str(k+1)  + '\n'			
  			sys.stderr.write(qq) 
  			
  			#we do not want to consider any trivial cases where terms cancel
  			if (elt == item or item == obj or elt == obj):
  				continue

			#F, G, and H now represent the added coefficient lists
  			F = [-x + y + z for x, y, z in zip(elt, item, obj)]  			
  			G = [x - y +z for x,y,z in zip(elt, item, obj)]
  			H = [x + y -z for x,y,z, in zip(elt, item, obj)]

		
  			F1 = np.array(F)
  			pr1 = False
  			#checks if coefficients are in range, alternating, ends in a 1, and isn't periodic
  			if ((np.all(F1 > -2) and np.all(F1 <2)) and checkAlt(F, n) and oddNumber(F,n) and ( not isPeriodic(F,n))):
				
				#normalizes the coefficients and returns F as a gap sequence list
  				F, corsum = normalize(F,n)
 
 				#checks to see if F is in the generating set and a small additional check (that the gaps add to n)
  				if ((F not in polys) and corsum): #and (F not in all)):
  					#prints out results
  					for q in range(len(F)):
  						print F[q],
  					print
  					#all.append(F)  					
  		
			#same as above
  			G1 = np.array(G)
  			pr2 = False
  			if ((np.all(G1 > -2) and np.all(G1 <2)) and checkAlt(G, n) and oddNumber(G,n) and (not isPeriodic(G,n))):
  			
  				G, corsum = normalize(G,n)
  				
  				if (corsum and (G not in polys)): #and (G not in all)):
  					for a in range(len(G)):
  						print G[a],
  					print
  					#all.append(G)
  			

			H1 = np.array(H)
  			pr3 = False
  			if ((np.all(H1 > -2) and np.all(H1 <2)) and checkAlt(H, n) and oddNumber(H,n) and ( not isPeriodic(H,n))):
  				
  				H, corsum = normalize(H,n)
  	
  				if (corsum and (H not in polys)): #and (H not in all)):
  					for z in range(len(H)):
  						print H[z],
  					print
  					#all.append(H)
  				
##############################################################
'''
normalize: inputs v (a coefficient list) and n (the length of the coefficient list) 
and returns a normalized gap list as well as a check variable, corsum.
gaps are normalized according to the known convention.
'''
def normalize(v, n): 
 #converts from a coefficient list to a gap sequence, called gaps
  gaps = []
  newv = []
  for i in range(len(v)):
  	newv.append(v[i])
  newv.append(-1)
  k = 1
   
  while (k <=n):
    gap = 0
    while (k<n and newv[k] == 0):
     	 gap += 1
      	 k += 1
    gaps.append(gap+1)
    k += 1

  sum = 0	
  for q in range(len(gaps)):
  	 sum += gaps[q]
  
  if (sum != n):
  	return gaps, False
  
  #finished setting up gap sequence [if result is 0 in gaps, that means there are less than 105 gaps]
  #finds the largest value in the array
  large = 0
  for h in range(len(gaps)):  
    if (gaps[h] > large):
      large  = gaps[h]
  
  #makes location, a list of all locations in gaps where largest values
  #elements are found  
  location = []
  for l in range(len(gaps)): 
    if (gaps[l] == large):
      location.append(l) 

  #winner contains the largest value element and direction for the normalized form
  winner = []
  winner.append(location[0]) #location of the largest value associated with the winner
  winner.append(1) #-1 means left, 1 means right

  #goes through all largest values and compares their left and right
  #sequences to one another, determines a winner, and then compares
  #the "internal" winner to the overall current winner
  for i in range(len(location)): 
    size  = len(gaps)
    val = location[i]
    val2 = location[i]

    while (True):
     
      prev = val
      prev2 = val2
      val += 1
      val2 -= 1
 
      if (val > size-1): #needs to wrap around to the beginning
        val = 0
      
      if (val2 < 0): #needs to wrap around to the end
        val2 = size-1
      
      #if val and val2 are equal -> an odd length symmetric gap sequence
      #arbitrarily chooses right
      if (val == val2):
      	 if (compareWithLargest(large, winner[0], winner[1], location[i], 1, gaps)):
          winner[0] = location[i]
          winner[1] = 1
        
         break
         
      #if val and val2 are equal to each others previous values implies
      #a even symmetric gap sequence
      #choose right arbitrarily 
      elif (val == prev2 and val2 == prev):
       	 	if (compareWithLargest(large, winner[0], winner[1], location[i], 1, gaps)):
          		winner[0] = location[i]
          		winner[1] = 1
    
        	break
      
      elif (gaps[val] > gaps[val2]): #so the right scan is better
        #print "right scan wins", gaps[val], ">", gaps[val2]
        if (compareWithLargest(large, winner[0], winner[1], location[i], 1, gaps)):
          winner[0] = location[i]
          winner[1] = 1
        
        break

      elif (gaps[val2] > gaps[val]): #so the left scan is better
        #print "left scan wins", gaps[val], "<", gaps[val2]
        if (compareWithLargest(large, winner[0], winner[1], location[i], -1, gaps) ):

          winner[0] = location[i]
          winner[1] = -1
        
        
        break
      
  #meantime will be the normalized gap sequence    
  meantime = []
  meantime.append(gaps[winner[0]])
  
  #adds values from gaps to meantime using the given direction
  #and starting location from winner
  if (winner[1] == 1): #if we are going right
    for k in range((winner[0]+1), len(gaps)): 
      meantime.append(gaps[k])
    
    for j in range(winner[0]): 
      meantime.append(gaps[j])

  if (winner[1] == -1): #if we are going left
    for i in range((winner[0]-1), -1, -1):
      meantime.append(gaps[i])
    
    for j in range((len(gaps)-1), winner[0], -1): 
      meantime.append(gaps[j]) 
  
  return meantime, True

'''
compareWithLargest: 
	an internal method to normalize that
	compares the current overall winner "largeloc, largedir" to
	the current location's local winner "comploc, compdir" and determines
	if the overall winner should be reset. 
	inputs: largest value, overall location, overall direction, comparing term
			   location, comparing term direction, gap sequence
'''
def compareWithLargest(large, largeloc, largedir, comploc, compdir, gaps): 
    
    
    #right is 1, left is -1
    size = len(gaps)
    val = largeloc
    val2 = comploc
    ret = False
    cycle = 0

    while (True):
      val = val + largedir
      val2 = val2 + compdir
      
      #need to account for going right or going left for each term
      if (val > size-1):
        val = 0
      

      elif (val < 0):
        val = size-1
      

      if (val2 > size-1):
        val2 = 0
      

      elif (val2 < 0):
        val2 = size-1
    

      if (gaps[val] > gaps[val2]): #the current largest wins
        return False
        
      
      elif (gaps[val2] > gaps[val]): #the comparison wins
        return True
        
      #if they are symmetric        
      if (val == largeloc and val2 == comploc):
        return True
    
'''
checkAlt: basic check to make sure that the coefficient
sequence always alternates sign
'''
def checkAlt(v, n):

	if (v[0] != 1):
	 return False
	sign = 1
	
	for i in range(1, n):
		if (v[i] == 0):
		 continue
		if (v[i] == sign):
		 return False
		if (v[i] == -sign):
			sign = -sign

	if (sign == -1):
		return False
	else:
		return True
	
'''
oddNumber: checks to make sure that the last
nonzero term in the sequence is a +1
'''
def oddNumber(v,n):
  i = n-1
  #print v[n-1]
  while (i > 0): 
    if (v[i] == 0):
      i -= 1
      continue
    elif (v[i] == -1):
      return False
    elif (v[i] == 1):
      return True
  return False

##############################################
'''
isPeriodic: checks to see if the coefficient sequence is periodic
'''
def isPeriodic(c, n):
    for t in range(2, n/2):
        if (hasPeriod(c, n, t)):
         return True
    
    return False

############################################
'''
hasPeriod: helper function for isPeriodic
'''
def hasPeriod(c,n,t):
    for i in range(n-t):
        if (c[i] != -c[i+t]):
         return False
    
    return True




main()



