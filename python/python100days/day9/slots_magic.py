#! -*-coding: utf-8 -*-
#!/usr/bin/python3 

class Person(object):
    __slots__ = ('_name','_age','_gender')
    
    def __init__(self,name,age):
        self._name = name 
        self._age = age 

    @property 
    def name(self):
        return self._name 
    
    @property 
    def age(self):
        return self._age 

    @age.setter 
    def age(self,age):
        self._age = age 
    
        

def main():
    person = Person('name',22)
    person._gender = "male"
    print(person._gender)


if __name__ == "__main__":
    main()
