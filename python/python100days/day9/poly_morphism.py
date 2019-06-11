#! -*- coding: utf-8 -*-

from abc import ABCMeta, abstractmethod


class Pet(object,metaclass = ABCMeta):
    """
    宠物抽象类
    """
    def __init__(self,nickname):
        self.nickname = nickname 
        
    @abstractmethod 
    def make_voice(self):
        pass 


class Cat(Pet):
    def  make_voice(self):
        print("喵喵喵？%s" %(self.nickname))



class Dog(Pet):
    def make_voice(self):
        print("汪汪汪？%s" %(self.nickname))


def main():
    pets = [Dog("旺财"),Cat("胖橘"),Dog("大黄")]
    for pet in pets:
        pet.make_voice()




if __name__ == "__main__":
    main()
        



        
