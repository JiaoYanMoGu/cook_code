def invert_num(num):
    result =0
    while num > 0 :
        result = result*10 + num%10 
        num //=10
    return  result 



if __name__ ==  '__main__':
    print(invert_num(123456))

