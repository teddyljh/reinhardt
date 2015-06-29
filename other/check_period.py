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
    
def test():
    a = [2,3,5,1,0,1,2,3,5,1,0,1,2,3,5,1,0,1]
    b = [1,0,1,2,3,5,1,0,1,2,3,5,1,0,1,2,3,5,6]
    
    print check_period(a)
    print check_period(b)

test()