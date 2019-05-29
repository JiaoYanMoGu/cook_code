#! -*-coding: utf-8 -*- 
#!/usr/bin/python3 

class Person(object):
    """
    class for @property and setter getter test 
    """
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
    
    @name.getter 
    def name(self):
        return self._name
    @name.setter 
    def name(self,name):
        if isinstance(name,str):
            self._name = name 
        else:
            print('Error name must be str')
def main():
    person= Person('王尼玛',11)
    person.age = 22
    person.name = "name"
    print(person.name)


if __name__ == '__main__':
    main()
