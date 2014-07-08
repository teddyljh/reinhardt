'''
constr_parse.py

this script takes output from results of the 
SRP2 construction and parses it into valid output for additional
programs. 

(c) Molly Feldman, Summer@ICERM, 2014. All Rights Reserved.
'''
import linecache
import sys


def main():
  filename = sys.argv[1]
  n = sys.argv[2] 
  n = int(n)

  # counts number of lines in file
  data = open(filename, 'r')
  data_str = data.readlines()



  for i in range(len(data_str)):
    val = []
    string = data_str[i].rstrip()
    for j in range(n):
      val.append(data_str[i][j])
    
    print "{",
    for k in range(len(val)):
      if (k == len(val)-1):
       ex = ""
      else:
        ex = ","
      if (val[k] == "0"): 
        print "0" + ex,
      elif (val[k] == "+"):
        print "1" + ex,
      
      elif (val[k] == "-"):
        print "-1" + ex,
    print "},"   
  

main()