import myvirtual

student = myvirtual.Student("Alice", 18)
student.study()
print(student)

teacher = myvirtual.Teacher("Mr. Smith", 187)
teacher.teach()
print(teacher)

myvirtual.call_invent(student)
myvirtual.call_invent(teacher)

class MyStudent(myvirtual.Student):
    def study(self):
        print("MyStudent is studying.")
ms1 = MyStudent("Bob", 20)
ms1.study()
print(ms1)
