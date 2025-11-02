import Myadd
result = Myadd.add_without_arg(3, 5)
print(result)
print("-----------------------------")
result = Myadd.add_with_arg(b=3, a=5)
print(result)
print("-----------------------------")
result = Myadd.add_with_default_arg(3)
print(result)

print(Myadd.char_a)
print(Myadd.char_b)