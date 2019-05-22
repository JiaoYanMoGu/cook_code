from invert_num  import *
def is_palindrome(num):
    if(invert_num(num)==num):
        return True 
    else:
        return False 

if __name__ == '__main__':
    print("1234321 is palindrome %d ",is_palindrome(1234321))
    print("123421 is palindrome %d ",is_palindrome(123421))
