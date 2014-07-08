'''
parse.py

script to determine all unique Reinhardt polynomials
given an output file containing an ordered combination 
of n=105. In other words, running output through this
script removes all dihedrally equivalent polynomials.

(c) Molly Feldman, Summer@ICERM, 2014. All Rights Reserved.
'''
import linecache

results = []

def main(): 
  # counts number of lines in file
  num_lines = sum(1 for line in open('cycle.out'))
  print num_lines
  for i in range(11, num_lines, 11):
      currline = linecache.getline('cycle.out', i)
      currline.replace(" ", "")
      for elt in results:
        if currline == elt: 
          continue
        else: 
          results.append(currline)

  count = 0
  for elt in results:
    print count
    print elt
    print


main()