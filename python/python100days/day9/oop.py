class Person(object):
    """
    Simple person  class 
    """
    def __init__(self,name,age):
        self._name = name 
        self._age = age 

    @property 
    def name(self):
        return self._name 
    
    @property 
    def age(self):
        return self.age 

    @name.setter 
    def name(self,name):
        self._name = name 

    @age.setter 
    def age(self,age):
        self._age = age 

    def play(self):
        print('%s is playing game!' % self._name)


class Student(Person):
    """
    class Student 
    """
    def __init__(self,name,age,grade):
        super().__init__(name,age)
        self._grade = grade 
    
    @property 
    def grade(self):
        return self._grade 
    
    @grade.setter
    def grade(self,grade):
        self._grade = grade 


def main():
    stu = Student('zhangsan',15,'grade3')
    stu.play()

if __name__ == '__main__':
    main()
