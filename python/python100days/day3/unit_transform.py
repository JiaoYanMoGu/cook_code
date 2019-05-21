#! -*- coding: utf-8 -*- 
#!/usr/bin/python3 

value = float(input("请输入长度："))
unit = input("请输入单位：")

if unit == 'in':
    print("%f 英寸 = %f 厘米"%(value,value*2.54))
elif unit =="cm":
    print("%f 厘米 = %f 英寸"%(value , value/2.54))
else:
    print("请输入有效的单位")
