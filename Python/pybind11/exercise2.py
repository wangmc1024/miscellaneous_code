import myclass
obj = myclass.Animals('cat',10)
print(obj)
print("--------------")
obj.demo = 100
print(obj)
print(obj.__dict__)
print(obj.demo)